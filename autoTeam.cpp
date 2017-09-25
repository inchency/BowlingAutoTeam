#include <iostream>
#include <fstream>
#include <algorithm>

#define MAX_SIZE 100

using namespace std;

class member {
public :
	int num; // ����
	char name[MAX_SIZE]; // �̸�
	int sixGameAvg = 0; // 6���� ����
	int totalAvg = 0; // ��Ż ����
};

class team {
public:
	int *num; // ���� ����
	int totalScore = 0; // �� ������ ���� ����
	int avgScore = 0; // �� ���� (�����Ƽ� ���� �̱���)
};

int totalNum(int n); // �������� �����ϴ� �� ȸ�� �� �Է¹ޱ�
void inputNameData(int n, member *m);
void readAvgFileData(int n, member *m); // ȸ���� AVG ������ ��� txt ������ �б�
void KDHAlgorithm(int n, member *m); // ���� ������ main �˰��� create date 2017-01-24
bool dec6Avg(class member a, class member b); // 6���� ������ �������� �������� ����

int main() {
	cout << "Made By\t �ڡ� KimDongHoon �١� \nVersion 1.0\t 2017-01-25" << endl;
	cout << "���ǻ��� : averageFile.txt �̶�� ȸ�� ���������� ��� �ؽ�Ʈ ������ �����ؾ��մϴ�. ����� ���� ������ ���Ƿ� ���Ͽ� ������ �����ϼ���!!" << endl;
	cout << "�ڵ� �� ���� ����� autoTeam.txt�� ����˴ϴ�." << endl;
	cout << "�ҽ��� ������̸�(���� ������ «..) ������ �ϰ�����ø� ���������� �����ּ��� ~.~" << endl << endl;
	int num = 0;
	num = totalNum(num);
	member *memberNum = new member[num]; // totalNum �Լ� ȣ����� member class ��ü �����Ҵ�
	inputNameData(num, memberNum);
	readAvgFileData(num, memberNum);
	sort(memberNum, memberNum + num, dec6Avg); // 6���� ������ �������� �������� �����ϱ�
	cout << "����\t�̸�\t6AGV\ttotalAVG" << endl;
	for (int i = 0; i < num; i++) {
		memberNum[i].num = i + 1; // ���� ����
		cout << memberNum[i].num << "\t" << memberNum[i].name << "\t" << memberNum[i].sixGameAvg << "\t" << memberNum[i].totalAvg << endl;
	}
	KDHAlgorithm(num, memberNum);
	char end;
	cout << "�ƹ�Ű�� �����ø� ����˴ϴ�." << endl;
	cin >> end;
	return 0;
}

int totalNum(int n) {
	cout << "�̹� �������� �����ϴ� ȸ���� ���� �Է��ϼ���." << endl;
	cin >> n;
	return n;
}

void inputNameData(int n, member *m) {
	for (int i = 0; i < n; i++) {
		cout << i + 1 << "��° �̸��� �Է��ϼ��� : ";
		cin >> m[i].name;
		ifstream instream("averageFile.txt");
		if (!instream.is_open()) {
			cout << "Error : averageFile.txt�� �� �� �����ϴ�." << endl;
			exit(1);
		}
		char firstLine[100];
		instream.getline(firstLine, 100);
		int num, sG, tA; // ����, 6���ӿ���, ��Ż����
		char name[MAX_SIZE]; // �̸�
		bool flag = false; // �Է��� �̸��� �ֳ� ���� �˻�
		while (instream >> num >> name >> sG >> tA) {
			if (!strcmp(m[i].name, name)) {
				strcpy_s(m[i].name, name);
				flag = true;
				break;
			}
		}
		if (!flag) { // �Է��� �̸��� ������
			cout << "�Է��� �̸��� �ٽ� Ȯ���ϼ���!!" << endl;
			i--;
		}
		instream.close();
	}
}

// Read Data File and input data
void readAvgFileData(int n, member *m) {
	ifstream instream("averageFile.txt");
	if (!instream.is_open()) {
		cout << "Error : averageFile.txt�� �� �� �����ϴ�." << endl;
		exit(1);
	}
	char firstLine[100];
	instream.getline(firstLine, 100);
	// cout << firstLine << endl;
	int num, sG, tA; // ����, 6���ӿ���, ��Ż����
	char name[MAX_SIZE]; // �̸�
	while (instream >> num >> name >> sG >> tA) {
		for (int i = 0; i < n; i++) {
			if (!strcmp(m[i].name, name)) {
				m[i].num = i + 1;
				m[i].sixGameAvg = sG;
				m[i].totalAvg = tA;
			}
		}
	}
	instream.close();
}
// main algorithm
void KDHAlgorithm(int n, member *m) {
	int teamN; // ���� �� ����
	cout << "�� ������ �������?" << endl;
	cin >> teamN;
	int teamNum, addNum; // �� ���� ���, �� ���������� �ʴ� �߰� ��� ��
	teamNum = n / teamN;
	addNum = n % teamN;
	if (addNum == 0) { // �� ������ ��������
		team *t = new team[teamN];
		// �� ������ ���� �����Ҵ��ϱ�
		for (int i = 0; i < teamN; i++)
			t[i].num = new int[teamNum];
		// ���� NULL�� �ʱ�ȭ
		for (int i = 0; i < teamN; i++) 
			for (int j = 0; j < teamNum; j++) 
				t[i].num[j] = NULL;
		// �� ������ ¥��
		for (int i = 0; i < teamN; i++) {
			int k = i + 1;
			for (int j = 0; j < teamNum; j++) {
				t[i].num[j] = k;
				k += teamN;
			}
		}
		ofstream outstream("autoTeam.txt");
		// �� ������ �̸� ���
		int k = 0;
		for (int j = 0; j < teamNum; j++) {
			for (int i = 0; i < teamN; i++) {
				if (k < n) {
					cout << m[t[i].num[j] - 1].name << "(" << m[t[i].num[j] - 1].sixGameAvg << ")" << "+" << m[t[0].num[j] - 1].sixGameAvg - m[t[i].num[j] - 1].sixGameAvg << "\t";
					outstream << m[t[i].num[j] - 1].name << "(" << m[t[i].num[j] - 1].sixGameAvg << ")" << "+" << m[t[0].num[j] - 1].sixGameAvg - m[t[i].num[j] - 1].sixGameAvg << "\t";
					k++;
				}
			}
			cout << endl;
			outstream << endl;
		}
		// �� ���� ��Ż ���� ���ϱ�
		for (int i = 0; i < teamN; i++)
			for (int j = 0; j < teamNum; j++)
				if (t[i].num[j] != 0)
					t[i].totalScore += m[t[i].num[j] - 1].sixGameAvg;
		// �� ���� ��Ż ���� ���
		cout << endl;
		cout << "�� ���� �հ� : ";
		outstream << endl;
		outstream << "�� ���� �հ� : ";
		for (int i = 0; i < teamN; i++) {
			cout << t[i].totalScore << " ";
			outstream << t[i].totalScore << " ";
		}
		cout << endl;
		outstream << endl;
		int max = 0;
		// ������ ���� ���� ���� ���� ���ϱ�
		for (int i = 0; i < teamN; i++)
			if (max <= t[i].totalScore)
				max = t[i].totalScore;
		cout << "�� �ڵ� ���� ���� �� : ";
		outstream << "�� �ڵ� ���� ���� �� : ";
		for (int i = 0; i < teamN; i++) {
			cout << "+" << max - t[i].totalScore << " ";
			outstream << "+" << max - t[i].totalScore << " ";
		}
		cout << endl;
		outstream << endl;
		outstream.close();
	}
	else { // �Ѹ��̶� ���� ����� �߻� �ϸ�
		team *t = new team[teamN];
		// �� ������ ���� �����Ҵ��ϱ�
		for (int i = 0; i < teamN; i++)
			t[i].num = new int[teamNum + 1];
		// ���� NULL�� �ʱ�ȭ
		for (int i = 0; i < teamN; i++)
			for (int j = 0; j < teamNum + 1; j++)
				t[i].num[j] = NULL;
		// �� ������ ¥��
		for (int i = 0; i < teamN; i++) {
			int k = i + 1;
			for (int j = 0; j < teamNum; j++) {
				t[i].num[j] = k;
				k += teamN;
			}
		}
		for (int k = 0, i = teamN*teamNum + 1; i <= teamN*teamNum + addNum; k++, i++)
			t[k].num[teamNum] = i;

		ofstream outstream("autoTeam.txt");
		// �� ������ �̸� ���
		int k = 0;
		for (int j = 0; j < teamNum + 1; j++) {
			for (int i = 0; i < teamN; i++) {
				if (k < n) {
					cout << m[t[i].num[j] - 1].name << "(" << m[t[i].num[j] - 1].sixGameAvg << ")" << "+" << m[t[0].num[j] - 1].sixGameAvg - m[t[i].num[j] - 1].sixGameAvg <<"\t";
					outstream << m[t[i].num[j] - 1].name << "(" << m[t[i].num[j] - 1].sixGameAvg << ")" << "+" << m[t[0].num[j] - 1].sixGameAvg - m[t[i].num[j] - 1].sixGameAvg << "\t";
					k++;
				}
			}
			cout << endl;
			outstream << endl;
		}
		// �� ���� ��Ż ���� ���ϱ�
		for (int i = 0; i < teamN; i++) 
			for (int j = 0; j < teamNum + 1; j++) 
				if(t[i].num[j] != 0)
					t[i].totalScore += m[t[i].num[j] - 1].sixGameAvg;
		// �� ���� ��Ż ���� ���
		cout << endl;
		cout << "�� ���� �հ� : ";
		outstream << endl;
		outstream << "�� ���� �հ� : ";
		for (int i = 0; i < teamN; i++) {
			cout << t[i].totalScore << " ";
			outstream << t[i].totalScore << " ";
		}
		cout << endl;
		outstream << endl;
		int max = 0;
		// ������ ���� ���� ���� ���� ���ϱ�
		for (int i = 0; i < teamN; i++) 
			if (max <= t[i].totalScore) 
				max = t[i].totalScore;
		cout << "�� �ڵ� ���� ���� �� : ";
		outstream << "�� �ڵ� ���� ���� �� : ";
		for (int i = 0; i < teamN; i++) {
			cout << "+" << max - t[i].totalScore << " ";
			outstream << "+" << max - t[i].totalScore << " ";
		}
		cout << endl;
		outstream << endl;
		outstream.close();
	}
}

bool dec6Avg(class member a, class member b) {
	return a.sixGameAvg > b.sixGameAvg;
}