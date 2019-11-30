#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>
using namespace std;

const int n = 9;

// ----- class 'Row' -----

class Row {
public:
	char b[16];
};

ostream&  operator<<(ostream& str, Row& r) {
	for (int i=0;i<n;i++) {
		str << (int)r.b[i];
	}
	return str;
}

bool operator<(Row& x, Row& y) {
	for (int i=0;i<n;i++) {
		if (x.b[i] < y.b[i])
			return true;
		if (x.b[i] > y.b[i])
			return false;
	}
	return false;
}

// ----- global vars -----

int diag_min = 285;
int diag_max = 285;

int background_min = 217;
int background_max = 218;

int equal_in_Row = 2;
int display_val = 8;

int cand_max = 10000000;

int histogram[n+1];
int Q[n+1] = {0,0,0,0,0,0,0,0,0,0};

int sol = 0;

Row * candidates[16];
Row test[16];

double determ_max = 0;
double determ_min = 1e80;

// ----- functions -----

double determ () {
	double a[n][n];
	for (int i=0;i<n;i++) {
		Row val = test[i];
		for (int j=0;j<n;j++) {
			a[i][j] = val.b[j]; 
		}
	}
	int i,j,k;
	double det=1; 
	int flag=0; 
   
	for (i=0;i<n;i++) {                //Pivotisation
        for (k=i+1;k<n;k++) {
            if (fabs(a[i][i])<fabs(a[k][i])){
            	flag++;
            	for (j=0;j<n;j++){
                    double temp=a[i][j];
                    a[i][j]=a[k][j];
                    a[k][j]=temp;
                }
			}
		}           
	}
    for (i=0;i<n-1;i++) {           //loop to perform the gauss elimination
		if (fabs(a[i][i])<1e-5)		// skip near-zero diagonal values
			return 0;
		for (k=i+1;k<n;k++) {
			double t=a[k][i]/a[i][i];
			for (j=0;j<n;j++)
				a[k][j]=a[k][j]-t*a[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
        }
	}
 	for(i=0;i<n;i++){
		det=det*a[i][i];
	}            
	if (flag%2==0){
		det=det;
	}else{
		det=-det;
	}
	return det;
}

void loadSettigs() {
	ifstream f("settings.txt");
	char  s[256];
	f >> s >> diag_min;
	f >> s >> diag_max;
	f >> s >> background_min;
	f >> s >> background_max;
	f >> s >> equal_in_Row;
	f >> s >> display_val;
	f >> s >> cand_max;
	f.close();

	cout << "diag_min:       " << diag_min << endl;
	cout << "diag_max:       " << diag_max << endl;
	cout << "background_min: " << background_min << endl;
	cout << "background_max: " << background_max << endl;
	cout << "equal_in_Row:   " << equal_in_Row << endl;
	cout << "display_val:    " << display_val << endl;
	cout << "cand_max:       " << cand_max << endl; 
}

int prod(Row& x, Row& y) {
	int o = 0;
	for (int i=0;i<n;i++) {
		o+= x.b[i] * y.b[i];
	}
	return o;
}

Row makeRow(vector<int>& v) {
	Row o;
	for (int i=0;i<n;i++) {
		o.b[i] = (char)v[i];
	}
	return o;
}

bool check(Row& x, int q) {
	for (int d=1;d<=9;d++) Q[d] = 0;
 	for (int i=0;i<9;i++) {
 		Q[x.b[i]]++;	
	}
	for (int i=1;i<=9;i++) {
		if (Q[i]>q)
			return false;
	}
	return true;
}

void updateHistogram(Row& x) {
	for (int i=0;i<n;i++) {
		histogram[x.b[i]]++;
	}
}

void releaseHistogram(Row& x) {
	for (int i=0;i<n;i++) {
		histogram[x.b[i]]--;
	}
}

void workWithSquaredMatrix()  {
	double det_o = determ();
	cout << "sol. " << ++sol << endl;
	cout << "det: " << det_o << endl;
	det_o = fabs(det_o);
	if (det_o > determ_max) determ_max = det_o;
	if (det_o < determ_min) determ_min = det_o;
}

void expand(int depth, int cand_o, int cand_size) {
	Row oldX;
	Row* cands = candidates[depth];
	Row* front = candidates[depth+1];
	int frontSize = 0;
	oldX = test[depth-1];
	int pr;
	for (int j = cand_o; j < cand_size; j++) {
		pr = prod(oldX, cands[j]);
		if (background_min<=pr && pr<=background_max) {
			front[frontSize++] = cands[j];
		}
		if (frontSize >= cand_max)
			break;
	}
	if (frontSize == 0)
		return;
	for (int i = 0; i < frontSize; i++) {
		test[depth] = front[i];
		updateHistogram(front[i]);

		bool dive = true;
		for (int h=1;h<=n;h++) {
			if (histogram[h]>n)
				dive = false;
		}
		if (dive) {
			if (depth+1 >= display_val) {
				cout << endl << depth+1 << ") " << hex <<endl;
				for (int i=0;i<=depth;i++) {
					cout << test[i]<<endl;			
				}
				cout << dec;
				if (depth + 1 >= n ) {
					workWithSquaredMatrix();
				}
				cout << "(max: " << determ_max << ")" << endl;
			}
			if (depth < n-1) {
				expand(depth+1, i+1, frontSize);
			}
		}
		releaseHistogram(front[i]);
	}
}

int makeInitialCandidateList(vector<Row> &v1, int i1, int i2) {
	vector<Row> v2;
	int s2 = 0;
	for (int j=i1;j<i2;j++) {
		vector<int> perm;
		for (int k=0;k<n;k++) {
			perm.push_back(v1[j].b[k]);
		}
		do {
			v2.push_back(makeRow(perm));
			s2++;	
			if (s2 >= cand_max)	break;
		} while (next_permutation(perm.begin(), perm.end()));
		if (s2 >= cand_max) break;
	}
	for (int c=1;c<=9;c++) {
		if (candidates[c]!= NULL) 
			delete[] candidates[c];
		candidates[c] = new Row[s2];
	}
	for (int r=0;r<s2;r++) 
		candidates[1][r] = v2[r];
	return s2;
}

int main() {
	loadSettigs();
	vector<Row> v1;
	Row q;

	clock_t time1 = clock();

	// ----- choose candidates for matrix rows (ignoring permutation of their elements)
	int a,b,c,d,e,f,g,h,i;
	int A,B,C,D,E,F,G,H,I;
	int sum;
	for (a=1;a<=9;a++) {A=a*a;
	for (b=a;b<=9;b++) {B=b*b;
	for (c=b;c<=9;c++) {C=c*c;
	for (d=c;d<=9;d++) {D=d*d;
	for (e=d;e<=9;e++) {E=e*e;
	for (f=e;f<=9;f++) {F=f*f;
	for (g=f;g<=9;g++) {G=g*g;
	for (h=g;h<=9;h++) {H=h*h;
	for (i=h;i<=9;i++) {I=i*i;
		sum = A+B+C+D+E+F+G+H+I; 
		if (diag_min<=sum && sum <=diag_max) {
			vector<int> v;
			v.push_back(a);	v.push_back(b);	v.push_back(c);	
			v.push_back(d);	v.push_back(e);	v.push_back(f);
			v.push_back(g);	v.push_back(h);	v.push_back(i);
			q = makeRow(v);
			if (check(q,equal_in_Row)) {
				v1.push_back(q);
			}
		}
	}}}}}}}}}
	int s1 = (int)v1.size();
	cout << "size1: " << s1 << endl;

	// move row '123456789' to the 1st place 
	int io = 0;
	for (int i=0;i<s1;i++) {
		if (check(v1[i],1))
			io = i;
	}
	Row r = v1[io];
	v1[io] = v1[0];
	v1[0] = r;
	
	for (int i=0;i<s1; i++) 
		cout << i+1 <<"] " << hex << v1[i] << dec << endl;
	
	// ----- search loop -----
	for (int h=0;h<=n;h++) histogram[h] = 0;
	for (int c=0;c<=n;c++)	candidates[c] = NULL;

	for (int i = 0; i < s1; i++) {
		test[0] = v1[i];
		updateHistogram(test[0]);
		int s2 = makeInitialCandidateList(v1, i, s1);
		cout << i+1 << "...) size2: " << s2 << endl;
	
		// recursively expand array 'test' (if possible) 
		expand(1, 0, s2);
		
		releaseHistogram(test[0]);
	}
	clock_t time2 = clock();
	cout << "{min: " << determ_min << "}" << endl;
	cout << "{max: " << determ_max << "}" << endl;
	cout << "made in " << (time2-time1)/(CLOCKS_PER_SEC*60.) << " min." << endl;
	return 0;
}
