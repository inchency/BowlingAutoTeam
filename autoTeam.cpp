#include <iostream>
#include <fstream>
#include <algorithm>

#define MAX_SIZE 100

using namespace std;

class member {
public :
	int num; // 연번
	char name[MAX_SIZE]; // 이름
	int sixGameAvg = 0; // 6게임 에버
	int totalAvg = 0; // 토탈 에버
};

class team {
public:
	int *num; // 팀의 연번
	int totalScore = 0; // 팀 에버의 총합 점수
	int avgScore = 0; // 팀 에버 (귀찮아서 아직 미구현)
};

int totalNum(int n); // 정기전에 참여하는 총 회원 수 입력받기
void inputNameData(int n, member *m);
void readAvgFileData(int n, member *m); // 회원의 AVG 정보가 담긴 txt 파일을 읽기
void KDHAlgorithm(int n, member *m); // 팀을 나누는 main 알고리즘 create date 2017-01-24
bool dec6Avg(class member a, class member b); // 6게임 에버를 기준으로 내림차순 정렬

int main() {
	cout << "Made By\t ★☆ KimDongHoon ☆★ \nVersion 1.0\t 2017-01-25" << endl;
	cout << "주의사항 : averageFile.txt 이라는 회원 에버정보가 담긴 텍스트 파일이 존재해야합니다. 기록이 없는 신입은 임의로 파일에 점수를 삽입하세요!!" << endl;
	cout << "자동 팀 배정 결과는 autoTeam.txt에 저장됩니다." << endl;
	cout << "소스는 비공개이며(워낙 더럽게 짬..) 수정을 하고싶으시면 개인적으로 연락주세용 ~.~" << endl << endl;
	int num = 0;
	num = totalNum(num);
	member *memberNum = new member[num]; // totalNum 함수 호출시켜 member class 객체 동적할당
	inputNameData(num, memberNum);
	readAvgFileData(num, memberNum);
	sort(memberNum, memberNum + num, dec6Avg); // 6게임 에버를 기준으로 내림차순 정렬하기
	cout << "순번\t이름\t6AGV\ttotalAVG" << endl;
	for (int i = 0; i < num; i++) {
		memberNum[i].num = i + 1; // 순번 리셋
		cout << memberNum[i].num << "\t" << memberNum[i].name << "\t" << memberNum[i].sixGameAvg << "\t" << memberNum[i].totalAvg << endl;
	}
	KDHAlgorithm(num, memberNum);
	char end;
	cout << "아무키나 누르시면 종료됩니다." << endl;
	cin >> end;
	return 0;
}

int totalNum(int n) {
	cout << "이번 정기전에 참여하는 회원의 수를 입력하세요." << endl;
	cin >> n;
	return n;
}

void inputNameData(int n, member *m) {
	for (int i = 0; i < n; i++) {
		cout << i + 1 << "번째 이름을 입력하세요 : ";
		cin >> m[i].name;
		ifstream instream("averageFile.txt");
		if (!instream.is_open()) {
			cout << "Error : averageFile.txt를 열 수 없습니다." << endl;
			exit(1);
		}
		char firstLine[100];
		instream.getline(firstLine, 100);
		int num, sG, tA; // 연번, 6게임에버, 토탈에버
		char name[MAX_SIZE]; // 이름
		bool flag = false; // 입력한 이름이 있나 없나 검사
		while (instream >> num >> name >> sG >> tA) {
			if (!strcmp(m[i].name, name)) {
				strcpy_s(m[i].name, name);
				flag = true;
				break;
			}
		}
		if (!flag) { // 입력한 이름이 없으면
			cout << "입력한 이름을 다시 확인하세요!!" << endl;
			i--;
		}
		instream.close();
	}
}

// Read Data File and input data
void readAvgFileData(int n, member *m) {
	ifstream instream("averageFile.txt");
	if (!instream.is_open()) {
		cout << "Error : averageFile.txt를 열 수 없습니다." << endl;
		exit(1);
	}
	char firstLine[100];
	instream.getline(firstLine, 100);
	// cout << firstLine << endl;
	int num, sG, tA; // 연번, 6게임에버, 토탈에버
	char name[MAX_SIZE]; // 이름
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
	int teamN; // 나눌 팀 갯수
	cout << "몇 팀으로 나눌까요?" << endl;
	cin >> teamN;
	int teamNum, addNum; // 한 팀의 명수, 딱 나누어지지 않는 추가 사람 수
	teamNum = n / teamN;
	addNum = n % teamN;
	if (addNum == 0) { // 딱 나누어 떨어지면
		team *t = new team[teamN];
		// 각 팀마다 연번 동적할당하기
		for (int i = 0; i < teamN; i++)
			t[i].num = new int[teamNum];
		// 연번 NULL로 초기화
		for (int i = 0; i < teamN; i++) 
			for (int j = 0; j < teamNum; j++) 
				t[i].num[j] = NULL;
		// 팀 구성원 짜기
		for (int i = 0; i < teamN; i++) {
			int k = i + 1;
			for (int j = 0; j < teamNum; j++) {
				t[i].num[j] = k;
				k += teamN;
			}
		}
		ofstream outstream("autoTeam.txt");
		// 팀 나누기 이름 출력
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
		// 팀 에버 토탈 점수 구하기
		for (int i = 0; i < teamN; i++)
			for (int j = 0; j < teamNum; j++)
				if (t[i].num[j] != 0)
					t[i].totalScore += m[t[i].num[j] - 1].sixGameAvg;
		// 팀 에버 토탈 점수 출력
		cout << endl;
		cout << "팀 에버 합계 : ";
		outstream << endl;
		outstream << "팀 에버 합계 : ";
		for (int i = 0; i < teamN; i++) {
			cout << t[i].totalScore << " ";
			outstream << t[i].totalScore << " ";
		}
		cout << endl;
		outstream << endl;
		int max = 0;
		// 팀에서 가장 높은 팀의 점수 구하기
		for (int i = 0; i < teamN; i++)
			if (max <= t[i].totalScore)
				max = t[i].totalScore;
		cout << "팀 핸디 점수 더할 값 : ";
		outstream << "팀 핸디 점수 더할 값 : ";
		for (int i = 0; i < teamN; i++) {
			cout << "+" << max - t[i].totalScore << " ";
			outstream << "+" << max - t[i].totalScore << " ";
		}
		cout << endl;
		outstream << endl;
		outstream.close();
	}
	else { // 한명이라도 남는 사람이 발생 하면
		team *t = new team[teamN];
		// 각 팀마다 연번 동적할당하기
		for (int i = 0; i < teamN; i++)
			t[i].num = new int[teamNum + 1];
		// 연번 NULL로 초기화
		for (int i = 0; i < teamN; i++)
			for (int j = 0; j < teamNum + 1; j++)
				t[i].num[j] = NULL;
		// 팀 구성원 짜기
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
		// 팀 나누기 이름 출력
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
		// 팀 에버 토탈 점수 구하기
		for (int i = 0; i < teamN; i++) 
			for (int j = 0; j < teamNum + 1; j++) 
				if(t[i].num[j] != 0)
					t[i].totalScore += m[t[i].num[j] - 1].sixGameAvg;
		// 팀 에버 토탈 점수 출력
		cout << endl;
		cout << "팀 에버 합계 : ";
		outstream << endl;
		outstream << "팀 에버 합계 : ";
		for (int i = 0; i < teamN; i++) {
			cout << t[i].totalScore << " ";
			outstream << t[i].totalScore << " ";
		}
		cout << endl;
		outstream << endl;
		int max = 0;
		// 팀에서 가장 높은 팀의 점수 구하기
		for (int i = 0; i < teamN; i++) 
			if (max <= t[i].totalScore) 
				max = t[i].totalScore;
		cout << "팀 핸디 점수 더할 값 : ";
		outstream << "팀 핸디 점수 더할 값 : ";
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