# LogEvo - The evolution of logger
![MIT License](https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge) 
![GitHub language count](https://img.shields.io/github/languages/count/gustavors1608/timered_embedded?style=for-the-badge) 
![Plataforma](https://img.shields.io/badge/Plataforma-ESP32-green?style=for-the-badge)


Biblioteca de logs para esp32, sendo a principal motivação para a criação da mesma, a problemática de realizar registro e monitoramento a distancia através de logs e registros de dados do sistema.

sendo assim se originou a LogEvo, uma lib que tem por objetivo principal realizar registro de mensagens através da serial e de arquivo spiffs, onde este pode ser requisitado e enviado para um servidor de monitoramento por exemplo.

sendo assim é possível acompanhar as features e suas características a seguir:

## Features
Algumas características do projeto:

- alta confiabilidade da informação, podendo reiniciar o device, ocorrer erros graves etc, e os dados ainda vão estar salvos na spiffs.
- simples utilização, com 4 comandos simplesmente ```LogEvo log(); , log.begin(115200); , log.set_time(timestamp) e log.set(local,tipo, mensagem)```
- arquivamento dos últimos 100 (ou mais) logs de erros em um arquivo spiffs.
- registro em formato de fácil compreensão e debug salvando o momento do registro, local no código, tipo do registro(informação, erro, warring, info importante...).
- criação de pilha de registro no spiffs de forma automática
- altamente personalizável, podendo alterar facilmente muitos itens.
- classificação e manipulação do arquivo de log, (contar linhas, deletar registro mais antigo etc...)
- registra o no documento de log somente apos o nivel 1, sendo assim não registra as mensagens de informação, somente as de warring, erros e informações importantes.
## Dicas

Uma delas, é a criação de **Macros**, onde estas são substituídas pelas suas definições, abaixo pode ser encontrado um exemplo:

```#define warring(x)     logger.set(__LINE__,WARRI,x);```

nesse caso ao chamar a macro de warring a seguir na linha 232:

``` warring("esta é uma mensagem de warring")```

será convertido para o seguinte apos compilado:

```logger.set(232,2,"esta é uma mensagem de warring");```

Assim facilitando a escrita e leitura, e melhorando até na manutenção do código claro.
## Firmware example

Exemplo de utilização da lib, nesse caso realiza os registros na serial alem do registros no arquivo spiffs.


```cpp / arduino

#include "LogEvo.cpp"

#define info(x)        logger.set(__LINE__,INFOR,x); //mostra uma informacao, exibinto tambem a linha do mesmo
#define warring(x)     logger.set(__LINE__,WARRI,x); //mostra um warring ou alerta 
#define important(x)   logger.set(__LINE__,IMPOR,x); //mostra uma informacao importante, exemplo, "firmware atualizado para v1.86.3"
#define error(x)       logger.set(__LINE__,ERROR,x); //mostra um erro do sistema

LogEvo logger(true); //o debug pela seria está ativo?

void setup() {
  //inicia ja informando a valocidade de comunicacao pela serial
  logger.begin(115200);


  // define o unix atual, para apartir disso a lib botar sempre o momento do erro
  logger.set_time(300000);

  //exemplos de uso
  info("mensagem de informação somente");
  warring("este é um warring");
  important("esta é uma informação importante !!!");
  error("deu ruim no sistema, esta é uma mensagem de erro");

}
void loop() {
}

```
após realizar o upload do código acima para seu esp 32, é só abrir a serial e ver as mensagens sendo registradas, veja também o arquivo: **```exemplos/basic.cpp```**

partiu evoluir a utilização dos seus registros de log? 




## License

este projeto é OpenSource e livre para a modificação, comercialização etc, que tal nos ajudar a melhorar esta lib?

[MIT](https://choosealicense.com/licenses/mit/)


## Author
Em caso de duvidas ou sugestões... ou queira entrar em contato:

- [Github: @gustavors1608](https://www.github.com/gustavors1608)

- [Instagram: @gustavo_stroschon](https://www.instagram.com/gustavo_stroschon)


Traduzido com a versão gratuita do tradutor - www.DeepL.com/Translator