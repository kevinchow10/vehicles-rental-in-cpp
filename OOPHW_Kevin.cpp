/**********************************************************************
Matr.-Nr:                     3117302
Nachname/Surname:             Kevin
Vorname/Given name:           Kevin
Uni-Email:                    kevin.kevin@stud.uni-due.de
Studiengang/Course of studis: ISE EE
**********************************************************************/
#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<string>
using namespace std;

class DayOfYear
{
    private:
        unsigned short int dayOfYear, year;
        static int days[];

    public:
        DayOfYear(int default_day=1, int default_year=365)
            : dayOfYear(default_day), year(default_year)
        {
        }

        DayOfYear(int day_const, int month_const, int year_const)
        {
            dayOfYear = day_const + days[month_const-1];
            year = year_const;
        }

        int get_dayOfYear()
        {
            return dayOfYear;
        }

        friend DayOfYear operator++(DayOfYear& day_tomorrow)
        {
            day_tomorrow.dayOfYear++;

            if(day_tomorrow.dayOfYear > 365)
            {
                day_tomorrow.year++;
                day_tomorrow.dayOfYear = 1;
            }
            
            return day_tomorrow;
        }

        friend istream& operator>>(istream& in, DayOfYear& day_input)
        {
            char dash;
            int day, month, year;

            in >> year >> dash >> month >> dash >> day;

            day_input.year = year;
            day_input.dayOfYear = day_input.days[month-1] + day;

            return in;
        }

        friend ostream& operator<<(ostream& out, const DayOfYear& day_output)
        {
            int day, month, i=0;

            while(day_output.dayOfYear > day_output.days[i])
            {
                month = i+1;
                i++;
            }

            day = day_output.dayOfYear - day_output.days[month-1];

            return out << day_output.year << '-' << setfill('0') << setw(2) << month << '-' << setfill('0') << setw(2) << day;
        }
};

int DayOfYear::days[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

class Vehicle
{
    private:
        const int no;
        string model;
        float price24h;

    public:
        Vehicle(int no_const, string model_const, float price_const)
            : no(no_const), model(model_const), price24h(price_const)
        {
        }

        ~Vehicle()
        {
        }

        int get_no()
        {
            return no;
        }

        string get_model()
        {
            return model;
        }

        float get_price(int rental_days)
        {
            return price24h * rental_days;
        }

        virtual void print() = 0;
};

class Bike:public Vehicle
{
    public:
        Bike(int no_const, string model_const)
            : Vehicle(no_const, model_const, 9.99)
        {
        }

        void print()
        {
            cout << get_model() << get_no() << " (Bike)";
        }
};

class EMotorVehicle:public Vehicle
{
    public:
        EMotorVehicle(int no_const, string model_const, float price_const)
            : Vehicle(no_const, model_const, price_const)
        {
        }

        virtual bool is_streetLegal() = 0;

        void print()
        {
            cout << get_model() << get_no();
            if(!is_streetLegal())
                cout << " (not street legal)";
        }
};

class EBike:public EMotorVehicle
{
    public:
        EBike(int no_const, string model_const, float price_const = 29.99)
            : EMotorVehicle(no_const, model_const, price_const)
        {
        }

        bool is_streetLegal()
        {
            return true;
        }

        void print()
        {
            EMotorVehicle::print();
            cout << " (EBike)";
        }
};

class EScooter:public EMotorVehicle
{
    private:
        bool streetLegal;
    
    public:
        EScooter(int no_const, string model_const, float price_const = 19.99, bool street_legal = false)
            : EMotorVehicle(no_const, model_const, price_const), streetLegal(street_legal)
        {
        }

        bool is_streetLegal()
        {
            return streetLegal;
        }

        void print()
        {
            EMotorVehicle::print();
            cout << " (EScooter)";
        }
};

class Rental
{
    private:
        const int no;
        DayOfYear from;
        int days;
        string customer;
        static int last_no;

    public:
        Rental(string customer_name, DayOfYear start_day, int no_of_days = 1)
            : customer(customer_name), from(start_day), days(no_of_days), no(last_no++)
        {
        }

        int get_days()
        {
            return days;
        }

        DayOfYear get_from()
        {
            return from;
        }

        DayOfYear get_until()
        {
            DayOfYear until = from;
            for(int i=1; i<=days; i++)
                ++until;
            return until;
        }

        // print after menu F
        void print()
        {
             cout << get_from() << " to " << get_until() << ", rental no. " << no << " for " << customer;
        }

};

int Rental::last_no = 1;

class Schedule
{
    private:
        Vehicle *vehicle;
        Rental *year[365];
        Schedule *next;

    public:
        Schedule(Vehicle *vehicle)
        {
            this->vehicle = vehicle;

            for(int y=0; y<365; y++)
                year[y] = nullptr;

            next = nullptr;
        }

        Vehicle* get_vehicle()
        {
            return vehicle;
        }

        Schedule* get_next()
        {
            return next;
        }

        void set_next(Schedule* schedule)
        {
            next = schedule;
        }

        bool isFree(DayOfYear start_day, int no_of_days)
        {
            for(int i=start_day.get_dayOfYear()-1; i<start_day.get_dayOfYear() + no_of_days; i++)
            {
                if(year[i] != nullptr)
                    return false;
            }
            return true;
        }

        float book(string customer_name, DayOfYear start_day, int no_of_days)
        {
            Rental* rental = new Rental(customer_name, start_day, no_of_days);
            for(int i=start_day.get_dayOfYear()-1; i<start_day.get_dayOfYear() + no_of_days; i++)
                year[i] = rental;

            return vehicle->get_price(no_of_days);
        }

        void print(DayOfYear date)
        {
            vehicle->print();
            
            if(year[date.get_dayOfYear()] != nullptr)
            {
                cout << endl;
                year[date.get_dayOfYear()]->print();
            }
                
        }

        void print()
        {
            vehicle->print();
            cout << endl;

            for(int i=1; i<365; i++)
            {
                if(year[i] != nullptr)
                {
                    year[i]->print();
                    cout << " , total: " << vehicle->get_price(year[i]->get_days()) << " EUR" << endl;
                    i = year[i]->get_until().get_dayOfYear();
                }
            }
        }
};

class Agency
{
    private:
        float profitPercent, profit;
        Schedule *head, *last;

    public:
        Agency()
            : profitPercent(0.2), profit(0.00), head(nullptr), last(nullptr)
        {
        }
        
        // menu G
        float get_profit()
        {
            return profit;
        }

        // menu H
        void set_profitPercentage(float profit_percent)
        {
            profitPercent = profit_percent / 100.00;
        }

        // menu I, J, and K
        void add(Vehicle* vehicle)
        {
            Schedule* schedule =  new Schedule(vehicle);
            schedule->set_next(nullptr);

            if(head == nullptr)
            {
                head = schedule;
                last = schedule;
            }

            else
            {
                last->set_next(schedule);
                last = last->get_next();
            }

        }

        bool isFree(int vehicle_no, DayOfYear start_day, int no_of_days)
        {
            Schedule* schedule;
            schedule = searchFor(vehicle_no);
            return schedule->isFree(start_day, no_of_days);
        }

        float book(int vehicle_no, string customer_name, DayOfYear start_day, int no_of_days)
        {
            Schedule *schedule;
            schedule = searchFor(vehicle_no);
            float total_price = schedule->book(customer_name, start_day, no_of_days);
            profit = profit + (profitPercent * total_price);
            return total_price ;
        }

        // menu C
        int chooseVehicle()
        {
            int i = 1;
            Schedule* display = head;
            while(display != nullptr)
            {
                cout << i << ": ";
                display->get_vehicle()->print();
                cout << endl;
                display = display->get_next();
                i++;
            }

            int vehicle_no;
            cout << "choose vehicle no: ";
            cin >> vehicle_no;
            return vehicle_no;
        }

        // menu D and E
        void print(DayOfYear date)
        {
            int i = 1;
            Schedule* schedule = head;
            while(schedule != nullptr)
            {
                cout << i << ": ";
                schedule->print(date);
                cout << endl;
                schedule = schedule->get_next();
                i++;
            }
        }
        
        //menu F
        void print()
        {
            int i = 1;
            Schedule* schedule = head;
            while(schedule != nullptr)
            {
                cout << "SCHEDULE FOR " << i << ": ";
                schedule->print();
                schedule = schedule->get_next();
                i++;
            }

            cout << "profit: " << fixed << setprecision(2) << get_profit() << " EUR";
        }

    protected:
        Schedule* searchFor(int vehicle_no)
        {
            Schedule* search = head;
            int count = 1;
            while(search != nullptr)
            {
                if(count == vehicle_no)
                    return search;
                count++;
                search = search->get_next();
            }
            return search;
        }
};

int main(void)
{
    char choice;
    int schedule_for_no, no, no_rental_bike = 1, no_rental_ebike = 1, no_rental_escooter = 1;
    DayOfYear today(1, 1, 2021);
    DayOfYear tomorrow = today;
    ++tomorrow;
    Agency agency;

    cout << "SIMPLIFIED SIMULATION OF A RENTAL AGENCY\n"
         << "========================================\n";
    
    do
    {
        cout << "\n----------------------------------------" << endl
             << "MENUE" << endl
             << "A end of simulation" << endl
             << "B set new simulation date" << endl
             << "C new rental manually" << endl
             << "D print rental of today " << today << endl
             << "E print rental of tomorrow " << tomorrow << endl
             << "F print all rentals" << endl
             << "G print agency profit" << endl
             << "H set agency profit percent" << endl
             << "I add bikes" << endl
             << "J add E-bikes" << endl
             << "K add E-scooters" << endl
             << "L new rental simulation" << endl
             << "your choice: ";
        cin >> choice;

        switch(choice)
        {
            case 'A':
            case 'a':
                return 0;
                
                cout << endl;
                system("pause");
                system("cls");
            break;
            
            case 'B':
            case 'b':
                cout << "input date of today: ";
                cin >> today;
                tomorrow = today;
                ++tomorrow;
                
                cout << endl;
                system("pause");
                system("cls");
            break;

            case 'C':
            case 'c':
            {
                DayOfYear date;
                int no_of_days;
                string name;
                int vehicle_no = agency.chooseVehicle();
                cout << "day of rental: ";
                cin >> date;
                cout << "number of days: ";
                cin >> no_of_days;
                
                while(no_of_days > 28)
                {
                    cout << "The maximum number of days chosen are 28 days. Please re-input again" << endl;
                    cout << "number of days: ";
                    cin >> no_of_days;
                }

                if(agency.isFree(vehicle_no, date, no_of_days))
                {
                    cout << "input data vehicle renter name: " << endl;
                    getline(cin >> ws, name);
                    cout << "booked, price for this rental: " 
                         << agency.book(vehicle_no, name, date, no_of_days) << " EUR";
                }

                else
                    cout << "sorry vehicle already booked";
                
                cout << endl;
                system("pause");
                system("cls");
                break;
            }

            case 'D':
            case 'd':
                cout << "SCHEDULE FOR " << today << endl;
                agency.print(today);
                
                cout << endl;
                system("pause");
                system("cls");
            break;

            case 'E':
            case 'e':
                cout << "SCHEDULE FOR " << tomorrow << endl;
                agency.print(tomorrow);
                
                cout << endl;
                system("pause");
                system("cls");
            break;

            case 'F':
            case 'f':
                agency.print();
                
                cout << endl;
                system("pause");
                system("cls");
            break;

            case 'G':
            case 'g':
                cout << "agency profit: " << fixed << setprecision(2) << agency.get_profit() << " EUR" << endl;
                
                system("pause");
                system("cls");
            break;

            case 'H':
            case 'h':
                float input_profit_percent;
                cout << "agency profit percent: ";
                cin >> input_profit_percent;
                agency.set_profitPercentage(input_profit_percent);
                
                cout << endl;
                system("pause");
                system("cls");
            break;

            case 'I':
            case 'i':
                cout << "How many Bikes in this simulation? ";
                cin >> no;
                
                for(int i=no_rental_bike; i<no_rental_bike + no; i++)
                    agency.add(new Bike(i, "City"));

                no_rental_bike+= no;

                cout << endl;
                system("pause");
                system("cls");
            break;

            case 'J':
            case 'j':
                cout << "How many EBikes in this simulation? ";
                cin >> no;
                
                for(int i=no_rental_ebike; i<no_rental_ebike + no; i++)
                    agency.add(new EBike(i, "Trekky"));

                no_rental_ebike+= no;

                cout << endl;
                system("pause");
                system("cls");
            break;

            case 'K':
            case 'k':
                cout << "How many EScooters in this simulation? ";
                cin >> no;
                
                for(int i=no_rental_escooter; i<no_rental_escooter + no; i++)
                    agency.add(new EScooter(i, "Scooty"));

                no_rental_escooter+= no;

                cout << endl;
                system("pause");
                system("cls");
            break;

            case 'L':
            case 'l':
            {
                int customer_no = 0, no_simulation, no = no_rental_bike + no_rental_ebike + no_rental_escooter - 3;
                cout << "how many reservations to simulate? ";
                cin >> no_simulation;

                for(int i=0; i<no_simulation; i++)
                {
                    int vehicle_no = 1 + (rand() % no), no_of_days = 1 + (rand() % 7);
                    DayOfYear date((1 + rand() % 365), 2021);
                    if(agency.isFree(vehicle_no, date, no_of_days))
                    {
                        customer_no++;
                        string name = "customer" + to_string(customer_no);
                        agency.book(vehicle_no, name, date, no_of_days);
                        cout << "\nrental vehicle " << vehicle_no << " from " << date << " for " << no_of_days << " days";
                    }

                    else
                    {
                        cout << "\nrental vehicle " << vehicle_no << " from " << date << " for " << no_of_days << " days";
                        cout << " sorry vehicle already booked";
                    }
                        
                }
                
                cout << endl;
                system("pause");
                system("cls");
                break;
            }

            default:
                cout << "Please input the choice between 'A' and 'L'" << endl;
                cout << endl;
                system("pause");
                system("cls");
        }
    } while (choice != 'A' || 'a');
}