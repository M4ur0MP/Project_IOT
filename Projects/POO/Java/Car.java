package Java;

public class Car {
    Integer id;
    String license;
    Account driver;
    Integer passengenger;

    public Car(String license, Account driver) {
        this.license = license;
        this.driver = driver;
    }

    @Override
    public String toString() {
        return "Car [driver=" + driver + ", id=" + id + ", license=" + license + ", passengenger=" + passengenger + "]";
    }

}
