# Manual das Interfaces e Protocolos

## Comunicação MQTT

O projeto "Smart Plant Monitoring with Automatic Irrigation" utiliza o protocolo MQTT para comunicação entre o dispositivo ESP32 e outros sistemas, permitindo monitoramento remoto e controle do sistema de irrigação.

### Configuração MQTT
- **Broker utilizado**: `broker.mqtt-dashboard.com`
- **Porta**: `1883`
- **Cliente ID**: `smart_plant_esp32`

### Tópicos de Publicação
O ESP32 publica dados nos seguintes tópicos:

- **`plant/soil`**: Valor de umidade do solo (leitura analógica)
- **`plant/temperature`**: Temperatura ambiente em °C (sensor DHT11)
- **`plant/humidity`**: Umidade relativa do ar em % (sensor DHT11)
- **`plant/light`**: Nível de luminosidade (leitura analógica do LDR)
- **`plant/pump`**: Estado da bomba de água (1 = ligada, 0 = desligada)

### Tópicos de Assinatura
O ESP32 assina os seguintes tópicos para receber comandos:

- **`plant/irrigate/force`**: Comando para forçar irrigação (1/on = ligar, 0/off = desligar)

### Frequência de Publicação
- Dados dos sensores são publicados a cada 30 segundos
- Estado da bomba é publicado imediatamente após qualquer mudança

### Formato das Mensagens
- Mensagens simples em formato de texto
- Valores numéricos são enviados diretamente (ex: "23.5" para temperatura)
- Comandos de controle utilizam "1"/"0" ou "on"/"off"

## Integração com Outros Sistemas

O protocolo MQTT permite fácil integração com:
- Dashboards de monitoramento (como Node-RED, Grafana)
- Aplicativos móveis
- Outros dispositivos IoT
- Plataformas de automação residencial

## Segurança

Na implementação atual, a comunicação MQTT é realizada sem criptografia ou autenticação, adequada apenas para ambientes de teste e desenvolvimento. Para implementações em produção, recomenda-se:

- Utilizar broker MQTT com suporte a TLS/SSL
- Implementar autenticação por usuário/senha
- Considerar o uso de certificados para autenticação de dispositivos
