objetivo = int(input('Escoge un numero: '))
epsilon = 0.001
paso = epsilon**2
respuesta = 0.0
iteracion = 0

while abs(respuesta**2 - objetivo) >= epsilon and respuesta <= objetivo:
    print(abs(respuesta**2-objetivo), respuesta)
    respuesta += paso
    iteracion += 1
print(f'Cantidad de iteraciones fueron {iteracion}')
if abs(respuesta**2 - objetivo) >= epsilon:
    print(f'No se encontra la raiz cuadrada {objetivo}')
else:
    print(f'La raiz cuadrada de {objetivo} es {respuesta}')
