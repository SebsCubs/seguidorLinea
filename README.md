# seguidorLinea
Codigo para QE16 seguidor de línea, vehículo de altas prestaciones.

# Comunicaciones

##Desde la interfaz hacia el microcontrolador
- Encender motores: (48)
- Apagar motores: (49)
- Constante de proporción M1 = (2 bytes)
- Constante de proporción M1 = (2 bytes)

##Desde el micro hacia la interfaz
- Paquetes de 22 bytes: un byte de inicio(00000000), 16 bytes de datos de 8 sensores, 4 bytes de datos de velocidad de motores, un byte de parada (11111111)
- Se enviará cada 1/50

# Pendientes
- Control usando datos de sensores y modulación PWM la cuál tiene retroalimentación con encoders de cuadratura.
- Envío de datos de velocidad y sensores infrarrojos para visualización con processing (SCI, Bluetooth).

Control de dirección
  - Control de velocidad para cada motor
  - Cambio de velocidad de los motores con base a la lectura de los sensores infrarrojos

Control de velocidad de motores.
- TPM1 & TPM2 para lectura de encoders.
- TPM3 para generación de center-aligned PWM.
- Cada vez que se lea un flanco de subida de los encoder guardar el valor de un contador de 16 bits a 31.25 kHz.
- Se usa el valor contado y la frecuencia para hallar los RPM (da 3 flancos de subida por vuelta).
- Se actualiza el valor del PWM para cada motor según una referencia, dado por los sensores infrarrojos
          
# Tareas
- David: Librerías con el ADC y TPM
- Paola: Processing y formato de envío
- Sebastian: Controlador de velocidad y dirección
* Deadline: 5/07/2019
