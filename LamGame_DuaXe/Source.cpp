#include<iostream>
#include<string>
#include<Windows.h>
#include<ctime>
#include<fstream>
#include <conio.h>
#include<mmsystem.h>

#pragma comment(lib, "winmm.lib")

using namespace std;

struct duongdua {
	int chisoduongdua;
	int mau_batdau;
	int mau_ketthuc;

};
typedef struct duongdua DUONGDUA;

struct xe {
	int hang;
	int cot;
};
typedef struct xe Xe;


char map[50][50];
void VeXePlayer(Xe);
bool KiemTraDungDo(Xe, int&, int&, int&);
int life = 0;
int point = 0;
int level = 0;
int bullet = 0;
int speed = 140;
int check_anMang = 0;  // để kiểm tra xem đã ăn được trái tim chưa, nếu rồi thì xoá trái tim đó đi
int check_dungdo = 0;
int check_anTien = 0;
int check_anBullet = 0;

int hang_mang = -10;
int cot_mang = 2 + rand() % (27 - 2 + 1);
int hang_tien = -20;
int cot_tien = 2 + rand() % (27 - 2 + 1);
int hang_dan = -1;
int cot_dan = 2 + rand() % (27 - 2 + 1);

int check_dan_bay;
int hang_dan_bay = -29;
int cot_dan_bay = -29;

//Hàm thay đổi kích cở của khung cmd
void resizeConsole(int width, int height) {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
void textcolor(int x) {
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}
void gotoxy(int x, int y) {
	HANDLE hConsoleOutput;
	COORD Cursor_an_Pos = { x - 1, y - 1 };
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}
void VeHinh(int x, int y, string str, int color) {
	gotoxy(x, y);   // di chuyển con trỏ
	textcolor(color); // tô màu
	cout << str;
	textcolor(7);   // Trở về như bình thường
}

void XoaManHinh() {
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}
void LoiGioiThieu() {
	resizeConsole(800, 600);

	ifstream FileIn;
	FileIn.open("GioiThieu.txt", ios_base::in);
	int time = 70;

	while (!FileIn.eof())
	{
		textcolor(14);
		char c;
		FileIn.get(c);
		Sleep(time);
		cout << c;

		if (_kbhit())
			time = 0;
	}

}
bool ChoiLai() {
	//resizeConsole(800, 600);

	VeHinh(30, 9, "                                    ", 112);
	VeHinh(30, 10, "                                    ", 112);
	VeHinh(30, 11, "                                    ", 112);
	VeHinh(30, 12, "                                    ", 112);
	VeHinh(30, 13, "                                    ", 112);
	VeHinh(30, 14, "                                    ", 112);
	VeHinh(30, 15, "                                    ", 112);
	VeHinh(30, 16, "                                    ", 112);
	VeHinh(30, 17, "                                    ", 112);

	VeHinh(32, 12, "THUA ROI SO DIEM CUA BAN LA: ", 112);
	gotoxy(61, 12); textcolor(112); cout << point; textcolor(7);
	VeHinh(37, 13, "Ban co muon choi lai?\n\n\n\n\n", 112);
	VeHinh(41, 15, "Yes", 112);
	VeHinh(51, 15, "No", 112);




	int check = 3;
	while (true)

	{
		if (GetAsyncKeyState(VK_RIGHT)) {
			VeHinh(41, 15, "Yes", 112);
			VeHinh(51, 15, "No\n\n\n\n\n\n\n\n\n\n", 118);
			check = 0;
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			VeHinh(41, 15, "Yes", 118);
			VeHinh(51, 15, "No\n\n\n\n\n\n\n\n\n\n", 112);
			check = 1;
		}
		if (GetAsyncKeyState(VK_RETURN)) {
			if (check == 1)
				return true;
			if (check == 0)
				return false;
		}
	}
}

// soKyHieu == 1 -> đường đua có ở trên, soKyHieu == 2 -> đường đua ko có ở trên
void VeDuongDua(int soKyHieu) {
	//dai 30, rong 30
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			if (soKyHieu == 1) {
				if (j == 14 && i % 2 == 0) {
					map[i][j] = '|';
				}
				else map[i][j] = ' ';
			}
			else if (soKyHieu == 2) {
				if (j == 14 && i % 2 != 0) {
					map[i][j] = '|';
				}
				else map[i][j] = ' ';
			}
		}
	}
}

void InDuongDua(DUONGDUA duongdua) {
	int dem = -1;
	int j;

	for (int i = 0; i < 30; i++) {
		cout << "\t\t\t\t";

		if (i < duongdua.chisoduongdua) {   // In Lề Đường ngoại lệ 
			for (j = 0; j < 30; j++) {
				if (j == 0 || j == 29) {
					textcolor(duongdua.mau_ketthuc);
					cout << map[i][j];
					textcolor(7);
				}
				else {
					if (j == 14 && map[i][j] == '|') {
						textcolor(14);
						cout << map[i][j];
						textcolor(7);
					}
					else
						//Tô màu xe player
						if (map[i][j] == 'X' || map[i][j] == '0' || map[i][j] == '#') {
							textcolor(6);
							cout << map[i][j];
							textcolor(7);
						}
					//tô màu xe computer
						else if (map[i][j] == '!') {
							textcolor(159);
							cout << " ";
							textcolor(7);
						}
					//tô màu trái tim
						else if (map[i][j] == 3) {
							textcolor(79);
							cout << map[i][j];
							textcolor(7);
						}
					//tô màu đạn
						else if (map[i][j] == 23) {
							textcolor(352);
							cout << map[i][j];
							textcolor(7);
						}
					//tô màu đạn bay
						else if (map[i][j] == '.') {
							textcolor(4);
							cout << map[i][j];
							textcolor(7);
						}
					//tô màu tiền
						else if (map[i][j] == '$') {
							textcolor(6);
							cout << map[i][j];
							textcolor(7);
						}
						else
							cout << map[i][j];
				}
			}
			cout << "\n";
		}
		else {						 // In lề đường chính thức
			dem++;
			for (j = 0; j < 30; j++) {
			quynhne:
				if (j == 0 || j == 29) {
					if (dem < 3) {
						textcolor(duongdua.mau_batdau);
						cout << map[i][j];
						textcolor(7);

					}
					else if (dem < 6) {
						textcolor(duongdua.mau_ketthuc);
						cout << map[i][j];
						textcolor(7);
					}
					else if (dem == 6) {
						dem = 0;
						goto quynhne;

					}
				}
				else {
					if (j == 14 && map[i][j] == '|') {
						textcolor(14);
						cout << map[i][j];
						textcolor(7);
					}
					else {
						//Tô màu xe player
						if (map[i][j] == 'X' || map[i][j] == '0' || map[i][j] == '#') {
							textcolor(6);
							cout << map[i][j];
							textcolor(7);
						}
						else
							//Tô màu xe
							if (map[i][j] == 'X' || map[i][j] == '0' || map[i][j] == '#') {
								textcolor(6);
								cout << map[i][j];
								textcolor(7);
							}
						//tô màu xe computer
							else if (map[i][j] == '!') {
								textcolor(159);
								cout << " ";
								textcolor(7);
							}
						//tô màu trái tim
							else if (map[i][j] == 3) {
								textcolor(79);
								cout << map[i][j];
								textcolor(7);
							}
						//tô màu đạn
							else if (map[i][j] == 23) {
								textcolor(352);
								cout << map[i][j];
								textcolor(7);
							}
						//tô màu đạn bay
							else if (map[i][j] == '.') {
								textcolor(4);
								cout << map[i][j];
								textcolor(7);
							}
						//tô màu tiền
							else if (map[i][j] == '$') {
								textcolor(6);
								cout << map[i][j];
								textcolor(7);
							}
							else
								cout << map[i][j];
					}
				}
			}
			cout << "\n";
		}
	}
}



//Toạ đọ xe trên cùng trái x = 2, y = 1
//Toạ đọ xe Dưới cùng trái x = 2, y = 28
//Toạ đọ xe trên cùng phải x = 27, y = 1
//Toạ đọ xe Dưới cùng phải x = 27, y = 28
void VeXePlayer(Xe xeplayer) {
	//thân xe
	map[xeplayer.hang][xeplayer.cot] = 'X';
	map[xeplayer.hang][xeplayer.cot - 1] = '#';
	map[xeplayer.hang][xeplayer.cot + 1] = '#';

	// bánh xe
	map[xeplayer.hang - 1][xeplayer.cot - 1] = '0';
	map[xeplayer.hang - 1][xeplayer.cot + 1] = '0';

	map[xeplayer.hang + 1][xeplayer.cot - 1] = '0';
	map[xeplayer.hang + 1][xeplayer.cot + 1] = '0';
}

void XoaXe(Xe& xe) {
	//thân xe
	map[xe.hang][xe.cot] = ' ';
	map[xe.hang][xe.cot - 1] = ' ';
	map[xe.hang][xe.cot + 1] = ' ';
	// bánh xe
	map[xe.hang - 1][xe.cot - 1] = ' ';
	map[xe.hang - 1][xe.cot + 1] = ' ';
	map[xe.hang + 1][xe.cot - 1] = ' ';
	map[xe.hang + 1][xe.cot + 1] = ' ';
}

bool check_pause = false;
// kiểm tra đụng độ để tránh sự bỏ qua của Hệ Điều Hành
void DiChuyenXe(Xe& xeplayer) {
	if (GetAsyncKeyState(VK_ESCAPE)) {
		exit(0);
	}
	if (GetAsyncKeyState(VK_RETURN)) {
		if (check_pause == true) {
			system("pause");
			check_pause = false;
		}
		else {
			check_pause = true;
			system("cls");
		}
	}

	if (GetAsyncKeyState(VK_LEFT)) {
		if (KiemTraDungDo(xeplayer, check_anMang, check_anTien, check_anBullet)) {
			Beep(600, 800); // 600 hertz  for 900 milliseconds     
		}
		if (xeplayer.cot != 2) {
			//Xoá xe đi
			XoaXe(xeplayer);
			//vẽ lại xe 
			xeplayer.cot--;
			VeXePlayer(xeplayer);
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT)) {
		if (KiemTraDungDo(xeplayer, check_anMang, check_anTien, check_anBullet)) {
			Beep(600, 800); // 600 hertz  for 900 milliseconds     
		}
		if (xeplayer.cot != 27) {
			XoaXe(xeplayer);
			//vẽ lại xe 
			xeplayer.cot++;
			VeXePlayer(xeplayer);
		}
	}
	else if (GetAsyncKeyState(VK_UP)) {
		if (KiemTraDungDo(xeplayer, check_anMang, check_anTien, check_anBullet)) {
			Beep(600, 800); // 600 hertz  for 900 milliseconds     
		}
		if (xeplayer.hang != 1) {
			XoaXe(xeplayer);
			//vẽ lại xe 
			xeplayer.hang--;
			VeXePlayer(xeplayer);
		}
	}
	else if (GetAsyncKeyState(VK_DOWN)) {
		if (KiemTraDungDo(xeplayer, check_anMang, check_anTien, check_anBullet)) {
			Beep(600, 800); // 600 hertz  for 900 milliseconds     
		}
		if (xeplayer.hang != 28) {
			XoaXe(xeplayer);
			//vẽ lại xe 
			xeplayer.hang++;
			VeXePlayer(xeplayer);
		}
	}
}

void VeXeChuongNgai(Xe xechuongngai) {
	//thân xe
	map[xechuongngai.hang][xechuongngai.cot] = '!';
	map[xechuongngai.hang][xechuongngai.cot - 1] = '!';
	map[xechuongngai.hang][xechuongngai.cot + 1] = '!';

	// bánh xe
	map[xechuongngai.hang - 1][xechuongngai.cot - 1] = '!';
	map[xechuongngai.hang - 1][xechuongngai.cot + 1] = '!';
	map[xechuongngai.hang + 1][xechuongngai.cot - 1] = '!';
	map[xechuongngai.hang + 1][xechuongngai.cot + 1] = '!';
}


void DiChuyen_TimDuong(bool& TimDuong) {
	if (TimDuong == true) {
		VeDuongDua(1);
		TimDuong = false;
	}
	else if (TimDuong == false) {
		VeDuongDua(2);
		TimDuong = true;
	}
}

bool KiemTraDungDo(Xe player, int& check_anMang, int& check_anTien, int& check_anbullet) {
	if (map[player.hang][player.cot] == 3 || map[player.hang][player.cot - 1] == 3
		|| map[player.hang][player.cot + 1] == 3 || map[player.hang - 1][player.cot - 1] == 3
		|| map[player.hang - 1][player.cot + 1] == 3 || map[player.hang + 1][player.cot - 1] == 3
		|| map[player.hang + 1][player.cot + 1] == 3) {
		life++;
		map[hang_mang][cot_mang] = ' ';
		check_anMang = 1;
	}
	if (map[player.hang][player.cot] == 23 || map[player.hang][player.cot - 1] == 23
		|| map[player.hang][player.cot + 1] == 23 || map[player.hang - 1][player.cot - 1] == 23
		|| map[player.hang - 1][player.cot + 1] == 23 || map[player.hang + 1][player.cot - 1] == 23
		|| map[player.hang + 1][player.cot + 1] == 23) {
		bullet++;
		map[hang_dan][cot_dan] = ' ';
		check_anBullet = 1;
	}
	if (map[player.hang][player.cot] == '$' || map[player.hang][player.cot - 1] == '$'
		|| map[player.hang][player.cot + 1] == '$' || map[player.hang - 1][player.cot - 1] == '$'
		|| map[player.hang - 1][player.cot + 1] == '$' || map[player.hang + 1][player.cot - 1] == '$'
		|| map[player.hang + 1][player.cot + 1] == '$') {
		point += 100;
		map[hang_tien][cot_tien] = ' ';
		check_anTien = 1;
	}
	if (map[player.hang][player.cot] == '!' || map[player.hang][player.cot - 1] == '!'
		|| map[player.hang][player.cot + 1] == '!' || map[player.hang - 1][player.cot - 1] == '!'
		|| map[player.hang - 1][player.cot + 1] == '!' || map[player.hang + 1][player.cot - 1] == '!'
		|| map[player.hang + 1][player.cot + 1] == '!') {
		life--;
		check_dungdo = 1;
		return true;
	}
	return false;
}

bool KiemTraBanDan(Xe xeplayer) {
	if (GetAsyncKeyState(VK_SPACE) && bullet > 0 && check_dan_bay == 0) {
		bullet--;
		hang_dan_bay = xeplayer.hang + 1;
		cot_dan_bay = xeplayer.cot;

		map[hang_dan_bay][cot_dan_bay] = '.';
		return true;
	}
	return false;
}

bool check_TrungDan_Tinh = false;
bool check_TrungDan_Tinh_2 = false;
bool check_TrungDan_Dong = false;
bool check_TrungDan_Dong_2 = false;

bool kiemtratrungdan(Xe& computer) {
	if ((cot_dan_bay == computer.cot || cot_dan_bay == computer.cot - 1 || cot_dan_bay == computer.cot + 1)) {
		if (hang_dan_bay == computer.hang + 1 || hang_dan_bay == computer.hang - 1 || hang_dan_bay == computer.hang) {
			XoaXe(computer);
			return true;
		}
	}
	return false;

}

int main() {
	do {
		LoiGioiThieu();

		cout << "\n\t\t    Nhan ENTER 2 lan de tiep tuc !";

		system("pause");
		cin.ignore(32767, '\n');
		if (GetAsyncKeyState(VK_RETURN)) {
			break;
		}
	} while (true);
	system("cls");


quynhne:
	
	resizeConsole(950, 599);
	bool isPlay = PlaySound(TEXT("haha.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	if (!isPlay) {
		cout << "Khong chay duoc";
	}

	int check_MapThu = 1;


	DUONGDUA duongdua1;
	duongdua1.chisoduongdua = 0;
	duongdua1.mau_batdau = 127;	 // màu trắng
	duongdua1.mau_ketthuc = 79;  //màu đỏ
	DUONGDUA duongdua2;
	duongdua2.chisoduongdua = 0;
	duongdua2.mau_batdau = 79;
	duongdua2.mau_ketthuc = 127;

	// VeDuongDua_1 -> có ở trên, VeDuongDua_2 -> không có ở trên  
	bool TimDuong = true; //true					//false


	Xe xe_player;
	xe_player.cot = 27, xe_player.hang = 28;

	Xe xe_chuongngai_Tinh;
	xe_chuongngai_Tinh.hang = 1;
	srand(time(0));
	xe_chuongngai_Tinh.cot = 2 + rand() % (27 - 2 + 1);   // random trong doan 2-26

	Xe xe_chuongngai_Tinh_2;
	xe_chuongngai_Tinh_2.hang = 1;
	srand(time(0));
	xe_chuongngai_Tinh_2.cot = 2 + rand() % (27 - 2 + 1);

	Xe xe_chuongngai_Dong;
	xe_chuongngai_Dong.hang = 1;
	srand(time(0));
	xe_chuongngai_Dong.cot = 2 + rand() % (27 - 2 + 1);

	Xe xe_chuongngai_Dong_2;
	xe_chuongngai_Dong_2.hang = 1;
	srand(time(0));
	xe_chuongngai_Dong_2.cot = 2 + rand() % (27 - 2 + 1);


	int dichchuyen;
	int dichchuyen_2;

	while (true)
	{
		if (life < 0) {
			break;
		}
		for (int chiso_LeDuong = 0; chiso_LeDuong < 3; chiso_LeDuong++) {
			if (life < 0) {
				break;
			}
			if (check_MapThu == 1) {
				duongdua1.chisoduongdua = chiso_LeDuong;
				DiChuyen_TimDuong(TimDuong);
				VeXePlayer(xe_player);

				if (check_dungdo == 0) {
					if (check_TrungDan_Tinh == false)
						VeXeChuongNgai(xe_chuongngai_Tinh);
					else {
						hang_dan_bay = -29;
						cot_dan_bay = -29;
					}
					if (speed < 50) {
						if (check_TrungDan_Tinh_2 == false)
							VeXeChuongNgai(xe_chuongngai_Tinh_2);
						else {
							hang_dan_bay = -29;
							cot_dan_bay = -29;
						}
					}
				}

				if (speed < 100) {
					//vẽ thêm mạng
					if ((speed < 100 && speed > 50) || (speed < -500))
					{
						if (check_anMang == 0)
							map[hang_mang][cot_mang] = 3;
					}
					if (speed < 0) {
						if (check_anTien == 0)
							map[hang_tien][cot_tien] = '$';
					}
					if (speed < 0) {
						if (check_anBullet == 0)
							map[hang_dan][cot_dan] = 23;
					}

					// vẽ thêm xe động
					srand(time(0));
					for (int i = 0; i < 10; i++)
						dichchuyen = -1 + rand() % 3; //random trong khoảng -1 -> 1
					int check = xe_chuongngai_Dong.cot + dichchuyen;

					srand(time(0));
					for (int i = 0; i < 10; i++)
						dichchuyen_2 = -1 + rand() % 3;
					int check_2 = xe_chuongngai_Dong_2.cot + dichchuyen_2;

					// điều kiện để xe được chuyển động:
					//dk1: xe nằm trong khoảng của đường chạy
					if (check >= 2 && check <= 27) {
						//dk2: 2 xe chạy không được chồng lên nhau
						if ((check < xe_chuongngai_Tinh.cot - 2) || (check > xe_chuongngai_Tinh.cot + 2)) {
							xe_chuongngai_Dong.cot = check;
						}
					}
					if (check_2 >= 2 && check_2 <= 27) {
						if ((check < xe_chuongngai_Tinh_2.cot - 2) || (check > xe_chuongngai_Tinh_2.cot + 2)) {
							xe_chuongngai_Dong_2.cot = check_2;
						}
					}
					if (check_dungdo == 0) {
						if (check_TrungDan_Dong == false)
							VeXeChuongNgai(xe_chuongngai_Dong);
						else {
							hang_dan_bay = -29;
							cot_dan_bay = -29;
						}
						if (speed < -300)
							if (check_TrungDan_Dong_2 == false)
								VeXeChuongNgai(xe_chuongngai_Dong_2);
							else {
								hang_dan_bay = -29;
								cot_dan_bay = -29;
							}
					}
				}
				if (check_dan_bay == 1) {
					map[hang_dan_bay][cot_dan_bay] = '.';
				}
				InDuongDua(duongdua1);
			}
			else if (check_MapThu == 2) {
				duongdua2.chisoduongdua = chiso_LeDuong;

				DiChuyen_TimDuong(TimDuong);
				VeXePlayer(xe_player);

				if (check_dungdo == 0) {
					if (check_TrungDan_Tinh == false)
						VeXeChuongNgai(xe_chuongngai_Tinh);
					else {
						hang_dan_bay = -29;
						cot_dan_bay = -29;
					}
					if (speed < 50) {
						if (check_TrungDan_Tinh_2 == false)
							VeXeChuongNgai(xe_chuongngai_Tinh_2);
						else {
							hang_dan_bay = -29;
							cot_dan_bay = -29;
						}
					}
				}

				if (speed < 100) {
					//vẽ thêm mạng
					if ((speed < 100 && speed > 50) || (speed < -500))
					{
						if (check_anMang == 0)
							map[hang_mang][cot_mang] = 3;

					}
					if (speed < 0) {
						if (check_anTien == 0)
							map[hang_tien][cot_tien] = '$';
					}
					if (speed < 0) {
						if (check_anBullet == 0)
							map[hang_dan][cot_dan] = 23;
					}

					// vẽ thêm xe động
					srand(time(0));
					for (int i = 0; i < 10; i++)
						dichchuyen = -1 + rand() % 3; //random trong khoảng -1 -> 1
					int check = xe_chuongngai_Dong.cot + dichchuyen;

					srand(time(0));
					for (int i = 0; i < 10; i++)
						dichchuyen_2 = -1 + rand() % 3;
					int check_2 = xe_chuongngai_Dong_2.cot + dichchuyen_2;

					// điều kiện để xe được chuyển động:
					//dk1: xe nằm trong khoảng của đường chạy
					if (check >= 2 && check <= 27) {
						//dk2: 2 xe chạy không được chồng lên nhau
						if ((check < xe_chuongngai_Tinh.cot - 2) || (check > xe_chuongngai_Tinh.cot + 2)) {
							xe_chuongngai_Dong.cot = check;
						}
					}
					if (check_2 >= 2 && check_2 <= 27) {
						if ((check < xe_chuongngai_Tinh_2.cot - 2) || (check > xe_chuongngai_Tinh_2.cot + 2)) {
							xe_chuongngai_Dong_2.cot = check_2;
						}
					}
					if (check_dungdo == 0) {
						if (check_TrungDan_Dong == false)
							VeXeChuongNgai(xe_chuongngai_Dong);
						else {
							hang_dan_bay = -29;
							cot_dan_bay = -29;
						}

						if (speed < -300)
							if (check_TrungDan_Dong_2 == false)
								VeXeChuongNgai(xe_chuongngai_Dong_2);
							else {
								hang_dan_bay = -29;
								cot_dan_bay = -29;
							}
					}
				}
				if (check_dan_bay == 1) {
					map[hang_dan_bay][cot_dan_bay] = '.';
				}
				InDuongDua(duongdua2);
			}


			// xe mới chướng ngại xuất hiện, sau khi xe cũ chạy hết map
			xe_chuongngai_Dong.hang++;
			xe_chuongngai_Tinh.hang++;
			xe_chuongngai_Dong_2.hang++;
			xe_chuongngai_Tinh_2.hang++;
			//Tăng điểm
			point++;


			//Sinh ra xe moi
			if (xe_chuongngai_Dong.hang == 32) {
				check_TrungDan_Dong = false;
				check_dungdo = 0;

				srand(time(0));
				for (int i = 0; i < 10; i++)
					xe_chuongngai_Dong.hang = -20 + rand() % 20;

				srand(time(0));
				for (int i = 0; i < 10; i++)
					xe_chuongngai_Dong.cot = 2 + rand() % (27 - 2 + 1);
			} 
			if (xe_chuongngai_Dong_2.hang == 32) {
				check_TrungDan_Dong_2 = false;
				check_dungdo = 0;

				srand(time(0));
				for (int i = 0; i < 10; i++)
					xe_chuongngai_Dong_2.hang = -20 + rand() % 20;

				srand(time(0));
				for (int i = 0; i < 10; i++)
					xe_chuongngai_Dong_2.cot = 2 + rand() % (27 - 2 + 1);
			}
			if (xe_chuongngai_Tinh.hang == 32) {
				check_TrungDan_Tinh = false;
				check_dungdo = 0;

				srand(time(0));
				for (int i = 0; i < 10; i++)
					xe_chuongngai_Tinh.hang = -10 + rand() % 10;

				srand(time(0));
				for (int i = 0; i < 10; i++)
					xe_chuongngai_Tinh.cot = 2 + rand() % (27 - 2 + 1);


				//Tăng tốc độ, level
				level++;
				speed -= 20;
			}
			if (xe_chuongngai_Tinh_2.hang == 32) {
				check_TrungDan_Tinh_2 = false;
				check_dungdo = 0;

				srand(time(0));
				for (int i = 0; i < 10; i++)
					xe_chuongngai_Tinh_2.hang = -10 + rand() % 10;

				srand(time(0));
				for (int i = 0; i < 10; i++)
					xe_chuongngai_Tinh_2.cot = 2 + rand() % (27 - 2 + 1);
			}

			char hinhtraitim = 3;
			char hinhdan = 23;
			VeHinh(70, 3, "ESC	 - exit", 15);
			VeHinh(70, 4, "Enter	 - pause / play", 15);
			VeHinh(70, 28, "Bullet: ", 15);
			gotoxy(79, 28); cout << bullet << " " << hinhdan;
			VeHinh(70, 29, "Space - shoot bullet", 3);
			VeHinh(100, 27, "Life: ", 15);
			gotoxy(109, 27); cout << life << " " << hinhtraitim;
			VeHinh(100, 28, "Level: ", 15);
			gotoxy(109, 28); cout << level;
			VeHinh(100, 29, "Points: ", 15);
			gotoxy(109, 29); cout << point;
			XoaManHinh();

			if (KiemTraBanDan(xe_player) == true) {
				check_dan_bay = 1;
			}
			//nêu đạn được bắn
			if (check_dan_bay == 1) {
				hang_dan_bay--;
			}
			// đạn chạy hết map -> đạn không được bắn
			if (hang_dan_bay == -1) {
				check_dan_bay = 0;
			}

			DiChuyenXe(xe_player);
			if (KiemTraDungDo(xe_player, check_anMang, check_anTien, check_anBullet)) {
				Beep(600, 800); // 600 hertz  for 900 milliseconds
			}
			if (kiemtratrungdan(xe_chuongngai_Dong) == true) {
				check_TrungDan_Dong = true;
				check_dan_bay = 0;
			}
			if (kiemtratrungdan(xe_chuongngai_Dong_2) == true) {
				check_TrungDan_Dong_2 = true;
				check_dan_bay = 0;
			}
			if (kiemtratrungdan(xe_chuongngai_Tinh) == true) {
				check_TrungDan_Tinh = true;
				check_dan_bay = 0;
			}
			if (kiemtratrungdan(xe_chuongngai_Tinh_2) == true) {
				check_TrungDan_Tinh_2 = true;
				check_dan_bay = 0;
			}

			//cho trái tim chạy 
			if (check_anMang == 0)
				hang_mang++;
			if (hang_mang == 32) {
				srand(time(0));
				hang_mang = -29 + rand() % 29;
				srand(time(0));
				cot_mang = 2 + rand() % (27 - 2 + 1);

				check_anMang = 0;
			}
			//cho tiền chạy
			if (check_anTien == 0)
				hang_tien++;
			if (hang_tien == 32) {
				srand(time(0));
				hang_tien = -20 + rand() % (20);
				srand(time(0));
				cot_tien = 2 + rand() % (27 - 2 + 1);

				check_anTien = 0;
			}
			//cho đạn chạy
			if (check_anBullet == 0)
				hang_dan++;
			if (hang_dan == 32) {
				srand(time(0));
				hang_dan = -20 + rand() % (20);
				srand(time(0));
				cot_dan = 2 + rand() % (27 - 2 + 1);

				check_anBullet = 0;
			}


			if (speed <= 0) {
				Sleep(0);
			}
			else {
				Sleep(speed);
			}
		}
		if (check_MapThu == 1)
			check_MapThu = 2;
		else if (check_MapThu == 2)
			check_MapThu = 1;
	}




	if (ChoiLai()) {
		life = 0;
		point = 0;
		level = 0;
		speed = 140;
		bullet = 0;
		system("cls");
		goto quynhne;
	}

	system("pause");
	return 0;
}