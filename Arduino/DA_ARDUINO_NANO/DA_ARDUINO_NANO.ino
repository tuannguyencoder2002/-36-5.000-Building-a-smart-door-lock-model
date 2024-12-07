#define KHOA  A0  
#define KM    2  // Chân kiểm tra khuôn mặt
#define CT    3  // Chân kiểm tra mật khẩu
#define BD    4  // Chân báo động
#define COI   13 // Chân còi báo động
#define DONG  HIGH
#define MO    LOW

#define R1 12
#define R2 11
#define R3 10
#define R4 9
#define C1 8
#define B2 7
#define B3 6
#define B4 5

// Định nghĩa các chân cho các ký hiệu A, B, C, D
#define A 6
#define B 7
#define C 8
#define D 9

String mang[4];
String pass = "1234";
String so = "", mk = "", sttt = "";
int index = 0, tt = 0, tt2 = 0, bd = 0, sai = 0, tc = 0, lan_bd = 0;
unsigned long int time_coi = 0, time_bd = 0;

void setup(void) {
  Serial.begin(9600); 

  // Cấu hình các chân
  pinMode(A, OUTPUT);  
  pinMode(B, OUTPUT);  
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);  

  pinMode(B1, INPUT_PULLUP); 
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(B4, INPUT_PULLUP);
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);

  pinMode(CT, OUTPUT);
  digitalWrite(CT, HIGH); 
  
  pinMode(BD, OUTPUT);
  digitalWrite(BD, HIGH);

  pinMode(KM, INPUT_PULLUP);

  pinMode(KHOA, OUTPUT);
  digitalWrite(KHOA, DONG);

  pinMode(COI, OUTPUT);
  digitalWrite(COI, LOW);
}

void loop(void) {
  ban_phim();
  if (sai >= 1)  {
    bao_dong();
    digitalWrite(BD, LOW);
  }
  if (digitalRead(KM) == 0) {
    Serial.println("KHUON MAT DUNG");
  }
  if (tt == 1 || digitalRead(KM) == 0) {
    digitalWrite(KHOA, MO);
    Serial.println("MO CUA");
    tt = 0;
    delay(4000);
  }
  digitalWrite(KHOA, DONG);
  Serial.println("DONG CUA");
}

// Chương trình quét phím
void ban_phim() {
  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);
  digitalWrite(R3, HIGH);
  digitalWrite(R4, HIGH);

  if (digitalRead(C1) == 0)  {
    so = "1";
    mang[index] = so;
    index++;
    delay(300);
  }

  if (digitalRead(B2) == 0)  {
    so = "4";
    mang[index] = so;
    index++;
    delay(300);
  }
  if (digitalRead(B3) == 0)  {
    so = "7";
    mang[index] = so;
    index++;
    delay(300);
  }
  if (digitalRead(B4) == 0)  {
    so = "*";
    delay(300);
  }

  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);

  if (digitalRead(B1) == 0)  {
    so = "2";
    mang[index] = so;
    index++;
    delay(300);
  }
  if (digitalRead(B2) == 0)  {
    so = "5";
    mang[index] = so;
    index++;
    delay(300);
  }
  if (digitalRead(B3) == 0)  {
    so = "8";
    mang[index] = so;
    index++;
    delay(300);
  }
  if (digitalRead(B4) == 0)  {
    so = "0";
    mang[index] = so;
    index++;
    delay(300);
  }

  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);

  if (digitalRead(B1) == 0)  {
    so = "3";
    mang[index] = so;
    index++;
    delay(300);
  }
  if (digitalRead(B2) == 0)  {
    so = "6";
    mang[index] = so;
    index++;
    delay(300);
  }
  if (digitalRead(B3) == 0)  {
    so = "9";
    mang[index] = so;
    index++;
    delay(300);
  }
  if (digitalRead(B4) == 0)  {
    so = "#";
    delay(300);
  }

  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);

  if (digitalRead(B1) == 0)  {
    so = "A";
    if (tt2 == 1) {
      pass = mk;
      bat_coi(3);
      tt2 = 0;
    }
    delay(300);
  }
  if (digitalRead(B2) == 0)  {
    so = "B";
    digitalWrite(COI, LOW);
    sai = 0;
    lan_bd = 0;
    digitalWrite(BD, HIGH);
    delay(300);
  }
  if (digitalRead(B3) == 0)  {
    so = "C";
    mang[0] = " ";
    mang[1] = " ";
    mang[2] = " ";
    mang[3] = " ";
    index = 0;
    tt = 0;
    tt2 = 0;
    delay(300);
  }
  if (digitalRead(B4) == 0)  {
    Serial.println(mk);
    if (mk == pass) {
      Serial.print("DUNG");
      tt = 1;
      tt2 = 1;
      bat_coi(1);
      digitalWrite(CT, LOW);
    }
    else  {
      Serial.println("SAI");
      sai++;
      tt = 0;
      tt2 = 0;
      bat_coi(2);
    }
    delay(500);
    digitalWrite(CT, HIGH);
    mang[0] = " ";
    mang[1] = " ";
    mang[2] = " ";
    mang[3] = " ";
    index = 0;
  }
  if (index >= 4)  index = 0;
  mk = mang[0] + mang[1] + mang[2] + mang[3];
}

void bat_coi(int lan) {
  for (int k = 0; k < lan; k++) {
    digitalWrite(COI, HIGH);
    delay(200);
    digitalWrite(COI, LOW);
    delay(200);
  }
}

void bao_dong() {
  if(lan_bd == 0){
    time_bd = millis();
    lan_bd = 1;
  }
  if (millis() - time_bd <= 30000) {
    if (millis() - time_coi >= 200) {
      tc++;
      if (tc >= 2) tc = 0;
      time_coi = millis();
    }
    if (tc == 0) digitalWrite(COI, LOW);
    if (tc == 1) digitalWrite(COI, HIGH);
  }
  else  {
    digitalWrite(COI, LOW);
    lan_bd = 0;
    sai = 0;
    digitalWrite(BD, HIGH);
  }
}
