# Descrição do Hardware

O projeto "Smart Plant Monitoring with Automatic Irrigation" utiliza uma combinação de microcontrolador, sensores e atuadores para monitorar continuamente as condições da planta e controlar automaticamente a irrigação com base em múltiplos parâmetros ambientais, utilizando comunicação MQTT para monitoramento remoto.

## Componentes Utilizados

### ESP32
Microcontrolador com Wi-Fi e Bluetooth integrados, responsável por ler os valores dos sensores, processar os dados, controlar o acionamento da bomba de água e realizar a comunicação MQTT. Possui processador dual-core Tensilica LX6 com velocidade de até 240 MHz, diversos pinos GPIO, ADC, DAC, PWM, entre outros recursos.

### Sensor de Umidade do Solo
Utilizado para medir a umidade presente no solo. Fornece sinal analógico que varia conforme a resistência entre seus eletrodos. Quanto mais úmido estiver o solo, maior a condutividade e menor a resistência, refletindo-se em um valor analógico característico.

### LDR (Sensor de Luminosidade)
O LDR (Light Dependent Resistor) é um resistor que varia sua resistência de acordo com a intensidade luminosa do ambiente. Em ambientes mais claros, a resistência diminui, enquanto em ambientes mais escuros a resistência aumenta. Esse comportamento permite monitorar a quantidade de luz disponível para a planta.

### DHT11 (Sensor de Umidade e Temperatura)
Sensor digital que mede a temperatura e a umidade relativa do ar. Utiliza um termistor e um sensor capacitivo de umidade, enviando os dados de forma serial para o microcontrolador. Possui faixa de medição de temperatura aproximadamente entre 0°C e 50°C e faixa de medição de umidade entre 20% e 90%.

### Módulo Relé 5V
Responsável por atuar como um interruptor eletromecânico, permitindo que o ESP32 (com sinais de 3,3V) controle a mini bomba de água. Opera normalmente com 5V na bobina e possui isolamento elétrico entre a parte de controle e a carga.

### Mini Bomba Submersa
Atuador final que realiza a irrigação. É energizada quando o relé é acionado, bombeando água para o vaso ou canteiro. Opera em 5V e possui vazão suficiente para pequenas irrigações domésticas.

### Protoboard
Base utilizada para montagem temporária e organização das conexões elétricas entre os componentes. Possui furos organizados em linhas e colunas para inserção dos componentes e faixas de alimentação para VCC e GND.

### Fonte Ajustável para Protoboard
Converte tensões de entrada (geralmente 6-12V) em tensões de saída de 5V e/ou 3,3V, possibilitando alimentar os componentes do projeto de maneira estável e prática. Possui conexão direta sobre a protoboard e seletor para tensões de 5V ou 3,3V.

### Jumpers e Resistores
- **Jumpers**: Cabos utilizados para realizar as conexões entre os furos da protoboard, sensores, módulos e o microcontrolador.
- **Resistores**: Usados para limitar corrente, ajustar divisores de tensão (como no caso do LDR) e proteger pinos do microcontrolador.

## Conexões e Integração

### ESP32
- **Pino 34 (ADC)**: Conectado à saída analógica do sensor de umidade do solo
- **Pino 35 (ADC)**: Conectado ao divisor de tensão do LDR
- **Pino 26 (GPIO)**: Conectado à entrada do módulo relé
- **Pino 27 (GPIO)**: Conectado ao pino de dados do sensor DHT11
- **3.3V**: Alimentação dos sensores (exceto relé)
- **5V**: Alimentação do módulo relé
- **GND**: Conexão comum de terra

### Sensor de Umidade do Solo
- **VCC**: 3.3V do ESP32
- **GND**: GND
- **Saída Analógica**: Pino 34 (ADC) do ESP32

### LDR (Sensor de Luminosidade)
- Conectado em um divisor de tensão com resistor de 10kΩ
- **Saída do divisor**: Pino 35 (ADC) do ESP32

### DHT11 (Sensor de Umidade e Temperatura)
- **VCC**: 3.3V do ESP32
- **GND**: GND
- **Pino de Dados**: Pino 27 (GPIO) do ESP32

### Módulo Relé
- **IN**: Pino 26 (GPIO) do ESP32
- **VCC**: 5V
- **GND**: GND

### Mini Bomba Submersa
- **Alimentação**: Saída do módulo relé
- **GND**: GND comum

## Observações sobre a Escolha do Hardware

O ESP32 foi escolhido como plataforma principal devido à sua capacidade de processamento, conectividade Wi-Fi integrada e múltiplos pinos analógicos e digitais, permitindo a conexão direta de todos os sensores e atuadores sem a necessidade de componentes adicionais.

A integração de múltiplos sensores (umidade do solo, luminosidade, temperatura e umidade do ar) permite uma tomada de decisão mais inteligente sobre quando irrigar, evitando desperdício de água e garantindo condições ideais para o desenvolvimento da planta.

O protocolo MQTT foi implementado para permitir o monitoramento remoto e controle do sistema, possibilitando a integração com outros dispositivos e plataformas IoT, além de facilitar a expansão futura do projeto.
