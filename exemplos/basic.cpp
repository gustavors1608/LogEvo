#include <Arduino.h>
#include "LogEvo.cpp"

#define info(x)        logger.set(__LINE__,INFOR,x); //mostra uma informacao, exibinto tambem a linha do mesmo
#define warring(x)     logger.set(__LINE__,WARRI,x); //mostra um warring ou alerta 
#define important(x)   logger.set(__LINE__,IMPOR,x); //mostra uma informacao importante, exemplo, "firmware atualizado para v1.86.3"
#define error(x)       logger.set(__LINE__,ERROR,x); //mostra um erro do sistema

#define print(x)   logger.not_effect(x); //mostra somente a mensagem, sme quebra d elinha, horario ou nada



LogEvo logger(true); //o debug pela seria está ativo?

void setup() {
  //inicia ja informando a valocidade de comunicacao pela serial
  logger.begin(115200);


  // define o unix atual, para apartir disso a lib botar sempre o momento do erro
  logger.set_time(1667512893); // = 11/03/2022 10:01pm

  // pode se usar os metodos da lib ou uma macro criada a cima na linha 7
  logger.set(2,INFOR,"");
  // ou
  info("dispositivo iniciado \n");



  //exemplos de uso
  warring("este é um warring");
  important("esta é uma informacao importante !!!");
  error("deu ruim no sistema, esta é uma mensagem de erro");

  // outra funcao, é ver o tamanho do arquivo armazenado
  String teste = "Numero de linhas no arquivo de log = " + String(logger.lines_file())+"\n";
  info(teste.c_str());

  // exibe o conteudo do arquivo spiffs para log de erros (pode ser utilizado para enviar esse conteudo para um servidor por exemplo)
  info("logs ja salvos na spiffs: --------------------------------")
  print(logger.get().c_str());

}
void loop() {
}