# README.md

# Smart Plant Monitoring with Automatic Irrigation

Este projeto implementa um sistema de monitoramento de plantas com irrigação automática utilizando ESP32, sensores de umidade do solo, temperatura, umidade do ar e luminosidade, com comunicação via protocolo MQTT.

## Descrição

O sistema "Smart Plant Monitoring with Automatic Irrigation" integra sensores, um ESP32 e comunicação MQTT para garantir o cuidado ideal das plantas. O sistema monitora continuamente a umidade do solo, temperatura e luz ambiente, acionando automaticamente a irrigação quando necessário.

O projeto oferece uma solução escalável e flexível para agricultura inteligente, enfatizando aspectos práticos e educacionais da IoT.

## Componentes Principais

- ESP32 (microcontrolador com WiFi integrado)
- Sensor de umidade do solo
- Sensor de luminosidade (LDR)
- Sensor de temperatura e umidade (DHT11)
- Módulo relé 5V
- Mini bomba submersa
- Protoboard e componentes auxiliares

## Funcionalidades

- Monitoramento contínuo de umidade do solo, temperatura, umidade do ar e luminosidade
- Irrigação automática baseada em múltiplos parâmetros
- Comunicação MQTT para monitoramento remoto e controle
- Possibilidade de acionamento manual da irrigação via comandos MQTT
- Prevenção de irrigação em condições inadequadas (como baixa luminosidade)

## Arquivos do Projeto

- `CODIGO_ESP32.ino`: Código principal para o ESP32
- `Descrição do Hardware.md`: Documentação detalhada dos componentes e conexões
- `Documentação das Interfaces e Protocolos.md`: Detalhes sobre a comunicação MQTT

## Configuração

1. Conecte os componentes conforme descrito na documentação de hardware
2. Abra o arquivo `CODIGO_ESP32.ino` na Arduino IDE
3. Instale as bibliotecas necessárias (WiFi, PubSubClient, DHT)
4. Configure as credenciais WiFi e parâmetros MQTT no código
5. Faça upload do código para o ESP32
6. Monitore os dados via cliente MQTT ou dashboard

## Comunicação MQTT

O sistema publica dados nos seguintes tópicos:
- `plant/soil`: Umidade do solo
- `plant/temperature`: Temperatura ambiente
- `plant/humidity`: Umidade do ar
- `plant/light`: Nível de luminosidade
- `plant/pump`: Estado da bomba

E recebe comandos no tópico:
- `plant/irrigate/force`: Para controle manual da irrigação

## Baseado no Artigo

Este projeto foi desenvolvido com base no artigo "Smart Plant Monitoring with Automatic Irrigation" da Universidade Presbiteriana Mackenzie feito por Davi Ferreira Brito Lima.
