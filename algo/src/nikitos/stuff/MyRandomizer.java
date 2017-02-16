package nikitos.stuff;

import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.Random;

public class MyRandomizer {
    private static Random rnd = new Random();
    private static int studentsNum = 0;

    private static String names[] = {
            "John",
            "Sara",
            "Fill",
            "Sam",
            "Nick",
            "Lucas",
            "David",
            "Henry"
    };

    private static String surnames[] = {
            "Jones",
            "Johnson",
            "Smith",
            "Williams",
            "Davis",
            "Miller",
            "Wilson",
            "Brown",
            "James"
    };

    public static String getName(){
        return names[rnd.nextInt(names.length)];
    }

    public static String getSurname(){
        return surnames[rnd.nextInt(surnames.length)];
    }

    public static Gender getGender(){
        return rnd.nextBoolean() ? Gender.MALE : Gender.FEMALE;
    }

    public static GregorianCalendar getGregorianCalendar(int minYear,  int maxYear){
        int year = minYear + rnd.nextInt(maxYear - minYear + 1);
        int month = rnd.nextInt(12);

        GregorianCalendar calendar = new GregorianCalendar(year, month, 1);
        calendar.set(year, month, calendar.getActualMaximum(Calendar.DAY_OF_MONTH));
        return calendar;
    }

    public static Student getStudent(){
        return new Student(getName(), getSurname(), rnd.nextInt(7), ++studentsNum, getGender(), rnd.nextBoolean());
    }
}
