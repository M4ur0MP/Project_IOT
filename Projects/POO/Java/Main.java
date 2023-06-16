package Java;

class Main {

    public static void main(String[] args) {
        Car car = new Car("AMQ123", new Account("Amdres Herrera", "A12356"));
        car.passengenger = 4;

        System.out.println(car);
    }

}