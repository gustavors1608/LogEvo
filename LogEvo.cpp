/*
lib para registro de logs e informacoes do programa, algumas funcoes especiais sao o log na spiffs, telemetria, 
geracao de codigos de erro, niveis de erro e log sendo:
1 - informacional 
daqui pra baixo salva na flash já
2 - warring
3 - info importante
4 - erros

salva as mensagens no seguinte formato:
unix|id|code|'mensagem'

caracteristicas:
salva até 100 registros na flash, apartir disso apaga as primeiras
quando chegar em 101, apaga a primeira, igual uma fila
n pode, em hipotese nenhuma, perder os registros
em caso de erro na memoria etc, reiniciar a placa, dar led de erro etc

*/
#include "SPIFFS.h"
#include "Arduino.h"


#define INFOR 1
#define WARRI 2
#define IMPOR 3
#define ERROR 4

#define _name_file_log "/log.txt"

#define _number_max_registers 10





//log evolution
class LogEvo{
    private:
        bool _debuguer_is_active = true;
        unsigned long _unixtime = 0;
        
    public:
        LogEvo(bool active);
        ~LogEvo();
        void set(int id_unique,uint8_t type_log, const char * msg, bool can_new_line = true);
        void not_effect(const char* msg);
        String get();
        void begin(unsigned long baud_rate);
        void set_time(unsigned long unixtime);
        uint16_t lines_file();

    protected:
        byte num();
        void vrf_queue();
        unsigned long time_now();
        void deleteLine();

};

LogEvo::LogEvo(bool active){
    this->_debuguer_is_active = active;
}

LogEvo::~LogEvo(){}

void LogEvo::begin(unsigned long baud_rate){
    if (!Serial and this->_debuguer_is_active == true){
        Serial.begin(baud_rate);
    }
}
void LogEvo::set_time(unsigned long unixtime){
    if(unixtime != 0){
        this->_unixtime = unixtime;
    }
}
uint16_t LogEvo::lines_file(){
    if (!SPIFFS.begin()) {
        return 0;
    }
    File file = SPIFFS.open(_name_file_log, "r+");
    if (!file) {
        return 0;
    }
    uint32_t size = file.size();
    file.seek(0);
    uint16_t currentLine = 0;

    for (uint32_t i = 0; i < size; ++i) {
        char currentChar = (char)file.read();
        if (currentChar == '\n') {
          ++currentLine;
        }
    }
    file.close();
    SPIFFS.end();
    return currentLine;
}

void LogEvo::deleteLine() {
    if (!SPIFFS.begin()) {
        return;
    }

    File file = SPIFFS.open(_name_file_log, "r+");
    if (!file) {
        return;
    }

    uint32_t size = file.size();
    file.seek(0);
    byte caracteres_linha_zero = 0;

    for (uint16_t i = 0; i < size; ++i) {
        char currentChar = (char)file.read();
        if (currentChar == '\n') {
          caracteres_linha_zero = i;
          break;
        }
        yield();
    }

    uint32_t end_size = size - caracteres_linha_zero;
    file.seek(caracteres_linha_zero+1);
    String new_content = "";

    for (uint16_t i = 0; i < end_size; ++i) {
        char currentChar = (char)file.read();
        new_content += String(currentChar);
        yield();
    }
    file.close();

    File file1 = SPIFFS.open(_name_file_log, FILE_WRITE);
    if (!file1) {
        return;
    }
    
    if (!file1.print(new_content)){
        return;
    }

    file1.close();
    SPIFFS.end();
}




//return unix
unsigned long LogEvo::time_now(){
    static unsigned long last_millis;
    while (millis() - last_millis >= 1000) {
        this->_unixtime++;
        last_millis += 1000;	
    }
    return this->_unixtime;
}
void LogEvo::not_effect(const char* msg){
    if(_debuguer_is_active == true){
        Serial.print(msg);
    }
}
void LogEvo::set(int id_unique,uint8_t type_log, const char * msg, bool can_new_line){
    //converter para char, com a funcao printf
    String log_msg = String(this->time_now())+"|"+String(type_log)+"|"+String(id_unique)+"|"+String(msg);

    if(can_new_line == true){
        log_msg += "\n";
    }

    if(_debuguer_is_active == true){
        if(msg == ""){
            Serial.println();
        }
        Serial.print(log_msg);
    }

    if(type_log != 1){ //se nao for informativo
        bool is_saved = false;
        do{
            if(this->lines_file() != 0){
                if(!SPIFFS.begin(true)) {
                    is_saved = false;
                }
                File file = SPIFFS.open(_name_file_log, FILE_APPEND);
                if (!file) {
                    is_saved = false;
                }
                if (file.print(log_msg) == true){
                    //sucesso
                    is_saved = true;
                }else{
                    is_saved = false;
                }
                
                file.close();
                SPIFFS.end();
            }else{
                if(!SPIFFS.begin(true)) {
                    is_saved = false;
                }
                File file = SPIFFS.open(_name_file_log, FILE_WRITE);

                if (!file) {
                    is_saved = false;
                }
                if(file.print(log_msg) == true){
                    //sucesso
                    is_saved = true;
                }else{
                    is_saved = false;
                }
                
                file.close();
                SPIFFS.end();
            }
        } while (is_saved == true);
        
        //se estourou o timeout e n salvou dai n sei...        
        this->vrf_queue();
    }
}
 
String LogEvo::get(){

    bool is_read = false;
    uint8_t timeout = 0;
    String content = "";

    do{
        if(!SPIFFS.begin(true)) {
            is_read = false;
        }
        
        File file = SPIFFS.open(_name_file_log, FILE_READ);
        
        if(!file || file.isDirectory()){
            is_read = false;
        }

        content = file.readString();

        if(content != ""){
            is_read = true;
        }

        file.close();
        SPIFFS.end();

        timeout++;
    } while (is_read == false and timeout < 5);
    
   
    return content;
}
/// @brief se passou da quantidade de logs, exclui o conteudo das linhas que estao sobrando
void LogEvo::vrf_queue(){
    uint32_t lines = this->lines_file();

    if(lines > _number_max_registers){
        uint32_t lines_delete = lines - _number_max_registers;

        for(unsigned int i = 0; i < lines_delete; i++){
            this->deleteLine();
        }
    }

    


}