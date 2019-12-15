#include<iostream>
#include<vector>
#include<cmath>
#define j2h(x) (3.1415926*(x)/180.0)
//ë�ʳ�3cm
//��Ļ����  200  200
using namespace std;
class node
{
public:
	node(double x = 0, double y = 0, double z = 0, double i = 0) :x(x), y(y), z(z), num(i) {}
	void setxyz(double x, double y, double z) { this->x = x; this->y = y; this->z = z; }
	friend ostream& operator <<(ostream &os, const node &rls)
	{
		cout << "��" << rls.num << "�ڵ�x���꣺" << rls.x << "   y����:" << rls.y << "   z����:" << rls.z << endl;
		return os;
	}
	double x, y, z, num;   //node�Ŀռ������Լ����
	double r;  //�����Բ�İ뾶
	double delta;  //ë�ʱ�ѹ���leteral_line���쳤��   r+deltaΪ��Բ�볤��
	double c;      //ë�ʱ�ѹ�����Բ�����
};
class segment
{
public:
	segment(double length = 0, double angle = 0, double i = 0) :length(length), angle(angle), num(i) {}
	void setL(double L) { length = L; }
	void setAngle(double A) { angle = A; }
	friend ostream& operator <<(ostream &os, const segment &rls)
	{
		cout << "��" << rls.num << "�Ƕγ���:" << rls.length << "   ��б�Ƕȣ�" << rls.angle << endl;
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
		double orient = 0, double L = 3)                         // ���캯��������ë�ʳ�ʼ���ڵ�Ŀռ������Լ�ë���ܳ��Ⱥ��˶�����
		:node_num(N), X(x), Y(y), Z(z), move_orien(orient), total_length(L)
	{
		if (N == 0)return;
		double gongcha = -0.02*L;//���ù���dΪ0.4cm   �Ǽ��γɵȲ����зֲ�
		double a1 = (total_length - 0.5*gongcha*node_num*(node_num - 1)) / node_num;
		nodevec.push_back(node(x, y, z, 0));
		for (int i = 0; i < N; i++)                 //���ݵȲ����й������ɳ��ȵȲ�ֲ���segment��
		{
			static double total = 0;
			double ai = a1 + (i)*gongcha;
			total += ai;
			segment t_seg(ai, 0, i + 1);
			node t_node(x, y, z - total, i + 1);
			segvec.push_back(t_seg);
			nodevec.push_back(t_node);
		}

		for (int i = 0; i < nodevec.size(); i++)//����ÿһ���ڵ��Ӧ�Ľ���Բ����
		{
			nodevec[i].delta = 0;
			nodevec[i].r = 0;                 //�Լ��������ë��ͷ��ϸ�ĺ���
			nodevec[i].c = nodevec[i].r;     // ��ʼ״̬������ڰ뾶����Ϊû��ѹ��

		}
	}
	void rotate(double angle)
	{

	}
	void set_ori(double ang) { this->move_orien = ang; }    //�ı��˶�����

	void update(double X, double Y, double Z, double angle)   //���� �ռ�����
	{
		this->X = X; this->Y = Y; this->Z = Z;
		nodevec[0].x = X;
		nodevec[0].y = Y;
		nodevec[0].z = Z;
		if (Z < total_length)                //���ë�ʴ���ֽ��
			;                                 //�˴�Ӧ�õ����ⲿ��matlab�Ż����� û�����
		else
		{
			for (auto &x : segvec)
			{
				x.angle = 0;                  //���ë�ʸ��ڵ�Z������ڱ�ͷ���ȣ���ô��ͷһ������ֱ�ģ�������йǼ��νǶȹ��㣬������
			}
		}
		for (int i = 1; i <= node_num; i++)     //���ѭ���Ǹ��ݸ��ڵ�Ŀռ�����������еĽڵ�Ŀռ�����
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
		rotate(angle);              //  �任�˶��Ƕ�
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
	vector<node>nodevec;    //һ������������Žڵ�  
	vector<segment>segvec;  //һ������������ŹǼ��� 
	double X, Y, Z;// ��ͷ��������  ������Ϊ�о�ë���˶�λ�õĴ���
	double move_orien;//ë���˶��ķ���
	double node_num; //ë�ʹǼ�����Ŀ
	double total_length;//��ͷ�ܳ���
};
int main()
{
	maobi mm(8, 5, 5, 10, 45, 3);     //����һ��ë��  �����ֱ�Ϊ  �Ǽ�����8��  ��ά�ռ����꣨5,5,10��  ë���˶�����45��  ��ͷ����3cm
	mm.print_maobi();           //��ӡĿǰë����Ϣ
	mm.update(6, 8, 5, 0);         // ë���˶����ռ䣨6,8,5�����˶������Ϊ0��
	cout << "\n\n\n\n\n";
	mm.print_maobi();          //
	mm.segvec[1].setAngle(30);  // ����һ����30��
	mm.segvec[2].setAngle(30);  //  ���ڶ�����30��
	mm.segvec[3].setAngle(30);  //   ����������30��   ��ʱ���Ķ�֮��ĹǼ����Ѿ�ˮƽ
	mm.update(6, 8, 2.5, 0);    //���¿ռ�λ������6,8,2.5��
	cout << "\n\n\n\n\n";
	mm.print_maobi();

	system("pause");
}