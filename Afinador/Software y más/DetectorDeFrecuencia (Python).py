import sounddevice as sd
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
from scipy.fft import rfft,rfftfreq
from scipy.signal import find_peaks
import serial
import time


fs=44110 #Frecuencia de sampleo 1kHz
Duracion = 3  # Segundos que va a grabar el programa

#Grabamos el audio desde el microfono de la computadora
Grabacion = sd.rec(Duracion * fs, samplerate=fs, channels=2,dtype='float64')
print("Grabando")
sd.wait()

Transformada=rfft(Grabacion[:,0]) #Tomamos los datos generados por la grabación y le realizamos una transformada de Fourier

Valor_Absoluto_Transformada=np.abs(Transformada) #Tomamos el valor absoluto de la transformada de Fourier (recordar que la transformada
                                                 # tiene tanto parte real como compleja)


Frecuencias=rfftfreq(n=np.shape(Grabacion)[0], d=1/fs) #Creamos una lista con las frecuencias que aparecen en la transformada de Fourier


peaks, _ = find_peaks(Valor_Absoluto_Transformada, prominence=3,distance=50,threshold=20) #Buscamos los picos de la transformada de Fourier, los argumentos
                                                                                          #de la función find_peaks ayudan a que el algoritmo tenga
                                                                                          #un mejor criterio para decidir que picos importan mas

print(Frecuencias[peaks]) #Mostramos en pantalla la lista de picos que encuentra el algoritmo


FrecuenciaDetectada=Frecuencias[peaks][0] #Guardamos en una variable el primer pico de frecuencia (frecuencia fundamental), este dato
                                           #será enviado a la placa arduino
                                      
FrecuenciaDetectada=round(FrecuenciaDetectada,2) #Redondeamos el valor obtenido a dos decimales, a arduino no le gustó que le mandemos floats
                                                 # con infinitos decimales
                                                 

# Conectamos el arduino con Python

arduino_port = 'COM6' #IMPORTANTE: Elegir el puerto correspondiente 

ser = serial.Serial(arduino_port, 9600) #Establecemos la conexión serial

time.sleep(2) #Damos un tiempo breve para que la conexión se efectue correctamente

ser.write(str(FrecuenciaDetectada).encode()) #Enviamos el dato de la frecuencia detectada a la placa


ser.close()  #Desconectamos el arduino de Python



#### En caso de querer graficar la transformada y los picos encontrados ####

#plt.plot(Frecuencias[0:3000],Valor_Absoluto_Transformada[0:3000])
#plt.plot(Frecuencias[peaks][0:800],Valor_Absoluto_Transformada[peaks][0:800],"x")
#plt.show()
