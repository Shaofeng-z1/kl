#include <Eigen/Dense>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <omp.h>

using namespace Eigen;
using namespace std;

int main()
{
 int Xmin = 0;
 int Xmax = 100;
 int Ymin = 0;
 int Ymax = 100;
 int Lx = Xmax - Xmin;
 int Ly = Ymax - Ymin;

 double dx = 1.0;
 double dy = 1.0;

 int n;
 n = Ly / dy + 1;
 int m;
 m = Lx / dx + 1;
 const int mn=m * n;

 MatrixXd C(mn, mn);
 MatrixXd x(mn, 2);

 double lambdax = 1;
 double lambday = 1;
 double meanY = 0;
 double VarY = 1;

 int kl_num = 300;

 int a;
 int b;

 for (int i = 0; i < mn; i++) {
	 a = (i + 1) % m;
	 b = (i + 1) / m;
	 if (a == 0) {
		 x(i,0) = (m - 1) * dx + Xmin;
		 x(i,1) = (b - 1) * dy + Ymin;
	 }
	 else {
		 x(i,0) = (a - 1) * dx + Xmin;
		 x(i,1) = b * dy + Ymin;
	 }
 };

 clock_t start, end;
 start = clock();
#pragma warning(disable: 6993)
 omp_set_num_threads(16); //设置线程的个数
#pragma omp parallel for
 for (int i = 0; i < mn; i++) {
	 for (int j = 0; j < mn; j++) {
		 C(i,j) = exp(-(abs((x(i,0)) - (x(j,0))) / lambdax + (abs((x(i,0)) - (x(j,0)))) / lambday));
		 //存在这一行就不能开并行计算
		 //std::cout << "第" << omp_get_thread_num() <<"个线程" <<"在运行" << "第" << i <<"个计算" << std::endl;/
	 }
 }
 end = clock();
 cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;  //输出时间（单位：ｓ）

 return 0;
}
