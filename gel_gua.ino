// Programa : Sensor de temperatura DS18B20
// Autor : FILIPEFLOP
 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
 
// Porta do pino de sinal do DS18B20
#define ONE_WIRE_BUS 6
 
// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);
 
// Armazena temperaturas minima e maxima
float tempAlvo = 20;
float varianciaMAX = 5;
bool compressorATIVO = HIGH;

//Define a porta que fornecerá energia do potênciometro
#define POTEN_ENERGY 3

//Define a porta analogica de entrada do potênciometro
#define POTEN_IN_ANALOG A0

//Define a potencia maxima lida no potenciometro
#define MAX_POWER 850

//Define o pino de controle do relé
#define RELE 5

//Define o temporizador
#define SLEEP_TIME 2000

// Define a temperaturas maximas e  minimas globais

#define GLOBAL_MIN_TEMP 15

#define GLOBAL_MAX_TEMP 20
 
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;
 
// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);





void setup(void)
{
  pinMode(POTEN_ENERGY, OUTPUT);
  pinMode(RELE, OUTPUT);
  pinMode(POTEN_IN_ANALOG, INPUT);
  Serial.begin(9600);
  sensors.begin();
  // Localiza e mostra enderecos dos sensores
  Serial.println("Localizando sensores DS18B20...");
  Serial.print("Foram encontrados ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  if (!sensors.getAddress(sensor1, 0)) 
     Serial.println("Sensores nao encontrados !"); 
  // Mostra o endereco do sensor encontrado no barramento
  Serial.print("Endereco sensor: ");
  mostra_endereco_sensor(sensor1);
  Serial.println();
  Serial.println();
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
}
 
void mostra_endereco_sensor(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // Adiciona zeros se necessário
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
 
void loop()
{

  // Le a informacao do sensor
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(sensor1);

  // Pega a temparatura alvo pelo potenciomatro
  tempAlvo = GLOBAL_MAX_TEMP * analogRead(POTEN_IN_ANALOG)/MAX_POWER + GLOBAL_MIN_TEMP;
    

  // Compara a temperatura do sensor com a temperatura alvo e os estados do comprassor
  if (tempC > (tempAlvo+varianciaMAX)){
      compressorATIVO = LOW;
      
    }

   if(tempC < tempAlvo){
      compressorATIVO = HIGH;
    }
  
  digitalWrite(RELE,compressorATIVO);
  
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("               ");
  lcd.setCursor(0,0);
  lcd.print("CURRENT ");
  lcd.setCursor(8,0);
  lcd.print(tempC);
  lcd.setCursor(0,1);
  lcd.print("               ");
  lcd.setCursor(0,1);
  lcd.print("TARGET");
  lcd.setCursor(8,1);
  lcd.print(tempAlvo);
  lcd.setCursor(15,1);
  if(compressorATIVO == HIGH){
      
      lcd.print("/");      
    }
  lcd.print(".");
  
  Serial.print(analogRead(POTEN_IN_ANALOG));
  Serial.println();
  delay(SLEEP_TIME);
}
