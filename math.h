const double EPS = 0.0001;

int random(int m);

bool sign(double a);

enum Axis{X, Y,     Axis_SIZE};

class Vec{
	public:
        Vec();
        Vec(double, double);
        double* ptr[Axis_SIZE];
		double x, y;
		double mod();
		Vec operator+(Vec);
		Vec operator/(double);
		Vec operator*(double);
		Vec operator-(Vec);
		bool operator==(Vec);
		Vec reflect();
};

double smult(Vec, Vec);
double vmult(Vec, Vec);

bool intersect(Vec, Vec, Vec, Vec);

Vec rand_vec(Vec, double);

Vec rand_vec(Vec);
