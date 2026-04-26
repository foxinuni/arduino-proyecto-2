# Proyecto 2 - Arduino 101

El siguiente codigo hace parte de la entrega del proyecto #2 de mi clase de Arduino 101. 
El proyecto esta hecho en PlatformIO, todo el codigo fuente se puede encontrar en la carpeta de `src`.


# Diagrama de Flujo
A continuacion se muestra el diagrama de flujo del codigo implementado.

```mermaid
graph TD
    Start([Inicio]) --> Setup[<b>setup</b><br/>Inicializar Serial, Pines,<br/>Botones, Gate, Display,<br/>Buzzer y Temperatura]
    Setup --> Loop([<b>loop</b>])

    %% Entrada y Salida
    Loop --> CheckEntry{¿Boton Entrada<br/>presionado?}
    CheckEntry -- Sí --> Full{¿Contador < 15?}
    Full -- Sí --> Inc[Incrementar contador<br/>Abrir puerta<br/>Tocar Megalovania]
    Full -- No --> Alarm[Tocar alarma advertencia]
    
    CheckEntry -- No --> CheckLeave{¿Boton Salida<br/>presionado?}
    CheckLeave -- Sí --> Empty{¿Contador > 0?}
    Empty -- Sí --> Dec[Decrementar contador<br/>Abrir puerta]
    Empty -- No --> Alarm
    
    %% Lógica del Ventilador
    Inc --> ReadSensors
    Alarm --> ReadSensors
    Dec --> ReadSensors
    CheckLeave -- No --> ReadSensors[Leer Fotocelda y<br/>Sensor Temperatura]

    ReadSensors --> CalcThresh[Evaluar Umbrales:<br/>Temp > 28°C o<br/>Luz > 4.2V o<br/>Vehículos >= 7]
    
    CalcThresh --> FanOn{¿Algún umbral<br/>superado?}
    FanOn -- Sí --> CalcSpeed[Calcular Velocidad Máxima<br/>usando MAP_RANGE]
    CalcSpeed --> SetFan[Motor DC: ON con PWM]
    FanOn -- No --> StopFan[Motor DC: OFF]

    %% Finalización del Ciclo
    SetFan --> SerialMsg{¿Pasó 1<br/>segundo?}
    StopFan --> SerialMsg
    
    SerialMsg -- Sí --> Print[Imprimir estado en<br/>Monitor Serial]
    SerialMsg -- No --> Updates[update_melody<br/>update_gate]
    
    Print --> Updates
    Updates --> Loop
```
