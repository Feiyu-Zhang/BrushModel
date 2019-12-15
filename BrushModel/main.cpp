#include<iostream>
#include<vector>
#include<cmath>
#define j2h(x) (3.1415926*(x)/180.0)
//毛笔长3cm
//屏幕长宽  200  200
using namespace std;
class node
{
public:
	node(double x = 0, double y = 0, double z = 0, double i = 0) :x(x), y(y), z(z), num(i) {}
	void setxyz(double x, double y, double z) { this->x = x; this->y = y; this->z = z; }
	friend ostream& operator <<(ostream &os, const node &rls)
	{
		cout << "第" << rls.num << "节点x坐标：" << rls.x << "   y坐标:" << rls.y << "   z坐标:" << rls.z << endl;
		return os;
	}
	double x, y, z, num;   //node的空间坐标以及标号
	double r;  //横截面圆的半径
	double delta;  //毛笔被压扁后，leteral_line的伸长量   r+delta为椭圆半长轴
	double c;      //毛笔被压扁后，椭圆半短轴
};
class segment
{
public:
	segment(double length = 0, double angle = 0, double i = 0) :length(length), angle(angle), num(i) {}
	void setL(double L) { length = L; }
	void setAngle(double A) { angle = A; }
	friend ostream& operator <<(ostream &os, const segment &rls)
	{
		cout << "第" << rls.num << "骨段长度:" << rls.length << "   倾斜角度：" << rls.angle << endl;
		return os;
	}
	double length;
	double angle;
	double num;
};

class maobi
{
public:
	maobi(double N = 0, double x = 0, double y = 0, double z = 10,
		double orient = 0, double L = 3)                         // 构造函数，给出毛笔初始根节点的空间坐标以及毛笔总长度和运动方向
		:node_num(N), X(x), Y(y), Z(z), move_orien(orient), total_length(L)
	{
		if (N == 0)return;
		double gongcha = -0.02*L;//设置公差d为0.4cm   骨脊段成等差数列分布
		double a1 = (total_length - 0.5*gongcha*node_num*(node_num - 1)) / node_num;
		nodevec.push_back(node(x, y, z, 0));
		for (int i = 0; i < N; i++)                 //根据等差数列公差生成长度等差分布的segment段
		{
			static double total = 0;
			double ai = a1 + (i)*gongcha;
			total += ai;
			segment t_seg(ai, 0, i + 1);
			node t_node(x, y, z - total, i + 1);
			segvec.push_back(t_seg);
			nodevec.push_back(t_node);
		}

		for (int i = 0; i < nodevec.size(); i++)//生成每一个节点对应的截面圆参数
		{
			nodevec[i].delta = 0;
			nodevec[i].r = 0;                 //自己设计生成毛笔头粗细的函数
			nodevec[i].c = nodevec[i].r;     // 初始状态短轴等于半径，因为没有压扁

		}
	}
	void rotate(double angle)
	{

	}
	void set_ori(double ang) { this->move_orien = ang; }    //改变运动方向

	void update(double X, double Y, double Z, double angle)   //更新 空间坐标
	{
		this->X = X; this->Y = Y; this->Z = Z;
		nodevec[0].x = X;
		nodevec[0].y = Y;
		nodevec[0].z = Z;
		if (Z < total_length)                //如果毛笔穿过纸面
			;                                 //此处应该调用外部的matlab优化函数 没有完成
		else
		{
			for (auto &x : segvec)
			{
				x.angle = 0;                  //如果毛笔根节点Z坐标高于笔头长度，那么笔头一定是竖直的，因此所有骨脊段角度归零，不弯曲
			}
		}
		for (int i = 1; i <= node_num; i++)     //这个循环是根据根节点的空间坐标更新所有的节点的空间坐标
		{
			double angle_need_to_calculate = 0;
			for (int j = 0; j < i; j++)
			{
				angle_need_to_calculate += segvec[j].angle;
			}
			nodevec[i].z = nodevec[i - 1].z - segvec[i - 1].length*cos(j2h(angle_need_to_calculate));
			nodevec[i].y = nodevec[i - 1].y;
			nodevec[i].x = nodevec[i - 1].x - segvec[i - 1].length*sin(j2h(angle_need_to_calculate));
		}
		rotate(angle);              //  变换运动角度
	}
	void print_maobi()
	{
		cout << *this;
	}
	friend ostream& operator <<(ostream &os, const maobi &rls)
	{
		for (auto x : rls.nodevec)
		{
			cout << x;
		}
		cout << endl;
		for (auto x : rls.segvec)
		{
			cout << x;
		}
		cout << endl;
		return os;
	}
public:
	vector<node>nodevec;    //一个向量里面放着节点  
	vector<segment>segvec;  //一个向量里面放着骨脊段 
	double X, Y, Z;// 笔头根部坐标  将其作为研究毛笔运动位置的代表
	double move_orien;//毛笔运动的方向
	double node_num; //毛笔骨脊段数目
	double total_length;//笔头总长度
};
int main()
{
	maobi mm(8, 5, 5, 10, 45, 3);     //构造一个毛笔  参数分别为  骨脊段数8段  三维空间坐标（5,5,10）  毛笔运动方向45度  笔头长度3cm
	mm.print_maobi();           //打印目前毛笔信息
	mm.update(6, 8, 5, 0);         // 毛笔运动至空间（6,8,5），运动方向变为0度
	cout << "\n\n\n\n\n";
	mm.print_maobi();          //
	mm.segvec[1].setAngle(30);  // 将第一段弯30度
	mm.segvec[2].setAngle(30);  //  将第二段弯30度
	mm.segvec[3].setAngle(30);  //   将第三段弯30度   此时第四段之后的骨脊段已经水平
	mm.update(6, 8, 2.5, 0);    //更新空间位置至（6,8,2.5）
	cout << "\n\n\n\n\n";
	mm.print_maobi();

	system("pause");
}