from car import Car

if __name__ == "__main__":
    print("Hola Mundo")

    car = Car()
    car.license = "AMG123"
    car.driver = "Andres Herrera"
    car.passengenger = 4
    print(vars(car))

    car2 = Car()
    car2.license = "AMG1567"
    car2.driver = "Pablo Herrera"
    car2.passengenger = 4
    print(vars(car2))
