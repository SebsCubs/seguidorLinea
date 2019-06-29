# seguidorLinea
Codigo para QE16 seguidor de línea, vehículo de altas prestaciones.

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
David: Librerías con el ADC y TPM
Paola: Processing y formato de envío
Sebastian: Controlador de velocidad y dirección
