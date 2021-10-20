
# 1. 주제
![vision align](https://sat02pap001files.storage.live.com/y4mtJ93S4YBY1eVjjPoiy4tnV7AsW-u-8PmEYkR6CeRWklGMvTY2TkgKdgsHBcuWlEVNOaVrh-3GuWLzPwcgy5UGtuVqf4hhKiDtFFUEUHvGPalos_Ny3ZHPZn13hfYtrUOOEgdNuucgJYPiatygrhCrF93F4T0K3--8O23MS8i5h1zpr8w9u2VzjRth9J0IlXUPeK83HL5n6A3_USG1O7Z9g/%EB%B9%84%EC%A0%BC%EC%96%BC%EB%9D%BC%EC%9D%B8.PNG?psid=1&width=1000&height=750)

- 휴대폰 제조 과정 중 합착 공정 내 Vision Align을 위한 영상처리 로직 개발
- 합착 공정 : Glass와 Film을 붙히는 작업

## 영상 처리 과정

1. 전처리 : 이진화, 모폴로지 연산을 수행하여 노이즈를 제거
2. 본처리 : Hole 중심 찾기, 휴대폰 Film의 중심점을 찾아 위치를 맞춤
3. 후처리 : Txy 계산, 휴대폰 Film에 Glass를 정확하게 붙히기 위해 어느정도 이동해야하는가를 계산
  

# 2. 영상처리 기초 학습(전처리)

## BMP 파일 구조 파악

- BitCount 값에 따라 헤더의 크기가 변함
- 비트수 1, 4, 8 일때 Color Table이 존재
- 비트수 24 일때 (3 배수 + padding) x row
- 비트수 32 일때 (4 배수) x row  가 픽셀데이터의 크기

![Bmp 파일 구조](https://sat02pap001files.storage.live.com/y4mqOLkbYnZcNpcGe0s7cV7b5T28abmM3DNcWyA5MhnEbdhwD4reZ2EmUnFlwAAiEC_OW54btEU7XynL56gftq01959reXzLfUHVp-ZlcSP8h7iY5VaI6KEq-OGfEtJOZ2OT2efKO8vKzdRnoEvooiOz6aW7RNRSG_rv9jpQrSjL00CpPsdTtH2wdQVMtfkuqllaRo8tFN_aNGlgOuiy8fKBQ/bmp%ED%8C%8C%EC%9D%BC%20%EA%B5%AC%EC%A1%B0.PNG?psid=1&width=500&height=250)


## 이미지 그레이 스케일링

- 이미지 영상의 RGB값에 가중치를 주어서 1픽셀의 RGB값을 동일하게 바꾸는것
- RGB값을 GRAY 로 변환하는 다양한 계수가 존재함

|| 24Bit 트루 컬러 영상|8Bit 그레이 영상|
|--|--|--|
|데이터량|Width * Height * 3|Width * Height * 1|
 
- 데이터량이 24bit 영상이 3배더 크기 때문에 연산 시간도 3배 걸린다.
- 처리 속도를 높히기 위해 이진화, 그레이 스케일링을 한다.

![그레이 스케일링](https://sat02pap001files.storage.live.com/y4ml9rsP3SVNeEELV0JKUifHJzYY5IcVXpwudApVpuU0BT0uYdf70_uKxBR0MLmYxgVhVPYFWp4U9MrXj2P5_fm0w1GaUpKdxTXmB6srvG1liShHgcgOUjQihiTg03R6tAqgCg_Vgcs28_CjfsYTqiOp_0-dux9DuAlM0c6k01W-f1CruU2ZlpruAwnLkOMuCQsRO5dwv8xHvPVaJorv3BuJQ/%EA%B7%B8%EB%A0%88%EC%9D%B4%20%EC%8A%A4%EC%BC%80%EC%9D%BC%EB%A7%81.PNG?psid=1&width=1920&height=1839)

### 구현 코드
```c++
void DibGrayscale(MyImage& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	MyImage cpy = dib;
	dib.CreateGrayImage(w, h);

	BYTE** ptr1 = dib.GetPtr();
	RGBBYTE** ptr2 = cpy.GetRGBPtr();

	BYTE r, g, b;
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++)
		{
			r = ptr2[j][i].r;
			g = ptr2[j][i].g;
			b = ptr2[j][i].b;

			ptr1[j][i] = (BYTE)limit(RGB2GRAY(r, g, b));
		}
	}
}
```

## 이진화

- 이미지 영상의 모든 픽셀을 임계값을 기준으로 흑과 백(0과 1)으로만 표현
- 이진화는 영상안에 포함된 물체의 특징을 검출하기위해 사용

![이진화](https://sat02pap001files.storage.live.com/y4mU1wxyi2RbnJ5q8-nY7NebTd2CvD3gBdUL1EQ87SNb-pIIsscBVl78GQe0OIHP0u38OqowBXv2H484QBX14kXEGeS_rIRPgOexZxfUaiJ_52sPwLnc4XsyPNGOdsOze2rtTQX95A44_htr9vh-a-J_vKtgalm8OcmBcO27TCekM4_5CKZZoWi6pFcA74OJh_lNXGYOY9A0BokTerRkMPXuQ/%EC%9D%B4%EC%A7%84%ED%99%94.PNG?psid=1&width=1938&height=963)

### 구현 코드

```c++
void DibBinarization(MyImage& dib, int th)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
		{
            // 임계값을 기준으로
			ptr[j][i] = (ptr[j][i] > th) ? 255 : 0;
		}
}
```

## 모폴로지 연산(열림, 닫힘)

- 영상의 밝은 영역이나 어두운 영역을 축소, 확대하는 기법
- 영상의 노이즈를 효과적으로 제거하기위하여 사용
 


 ![침식과 팽창](https://sat02pap001files.storage.live.com/y4mN1a22wDFye7m-nd8jgHvoNaTPN-_Ts6qQuQWhpjBknHAdZvrYj_1jW3_NmYM6zLiKp8djIrMWAlw7f4fUsXZxveB6gIKVE3NEzK-r1DEBIsEEoJqM_htAlE6x8NsrJOa57Xc4YijZijMcU5P02fnCB2PhfHNKrlC-I0woV5QVm0_LJCDopP8IO1WibFIK6pjcsQZd-PZdVc9ZtsZj1_R3Q/%EC%B9%A8%EC%8B%9D%EA%B3%BC%20%ED%8C%BD%EC%B0%BD.PNG?psid=1&width=1011&height=619)
   

- 열림과 닫힘 연산은 침식과 팽창 연산을 결합한 형태
- 열림 연산은 밝은 영역에 나타난 미세한 조각을 제거하는 연산
- 닫힘 연산은 밝은 영역에 생긴 미세한 틈을 메우는 역할을 함

|열림 연산(침식->팽창)|이진화|닫힘(팽창->침식)|
|--|--|--|
|![열림연산](https://sat02pap001files.storage.live.com/y4m7lh1qod-kPhsc3H3gTZLhi9PWyKgh9y7GvvpNxNZEtotP5vb4KgPJppWWL0peG0Y84q1feJIrNFRyn5dq07lRrTxERdPaW8qdF2M9xGmS16ZIOF0eFAPLSFG_LeJpUs6qxbQ4ky8ftyRUdAgHiSqSLxIrmKB_S8ZqGLs3m9lQEm1TlTMt4x7hDTLvtiQYkgCKT59G6Umk7vmcVOAA08uew/%EB%A0%88%EB%82%98%20%EC%97%B4%EB%A6%BC%EC%97%B0%EC%82%B0.png?psid=1&width=380&height=403)|![이진화](https://sat02pap001files.storage.live.com/y4mKhWO3Xj6fMc-LSZ_Tp9rItBuwVcK60YU4eK6RNsPEQGL3RcH2Whg4UEj0mE71876Bv0u3t-4FFp09kmuGPyh3wS57o1VvAuhbIvKeItykp4C2rNuUGxIFghb2tsS_2mOWt0fUVQyL19iK7TZeKAvAIKf-_bl4km_3pNlgE-2n17s4MxJRCNqZhJjp42-_1tI4N7dezsiIT3daJ6mx2gLNg/%EB%A0%88%EB%82%98%20%EC%9D%B4%EC%A7%84%ED%99%94.png?psid=1&width=378&height=403)|![닫힘 연산](https://sat02pap001files.storage.live.com/y4mSuxH0ZeacBBk7KXZmT1DmgTwqg35TcBqeC7kJTIvkA0ylap1SL9SWdE1kY3NZ2zfF03mrFjNkIhgifwMEGjECRNRA3_g7JiGedLPDUcVcurOmTZAxXNgOwLFSWcMGf67AWr0CRZMnl803TFRvpt0Pk0F26TUdNn05Rt1-JKj7CLLwEL_ktRfYtdA0GcvZ4JWwzfSwReyjh6Sfxqx0k6Lng/%EB%A0%88%EB%82%98%20%EB%8B%AB%ED%9E%98%EC%97%B0%EC%82%B0.png?psid=1&width=380&height=403)|

### 구현 코드

```c++
void DibMorphologyErosion(MyImage& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	MyImage cpy = dib;;

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			if (ptr2[j][i] != 0)
			{
				if (ptr2[j - 1][i - 1] == 0 ||
					ptr2[j - 1][i] == 0 ||
					ptr2[j - 1][i + 1] == 0 ||
					ptr2[j][i - 1] == 0 ||
					ptr2[j][i + 1] == 0 ||
					ptr2[j + 1][i - 1] == 0 ||
					ptr2[j + 1][i] == 0 ||
					ptr2[j + 1][i + 1] == 0)
				{
					ptr1[j][i] = 0;
				}
			}
		}
}

void DibMorphologyDilation(MyImage& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	MyImage cpy = dib;;

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			if (ptr2[j][i] == 0)
			{
				if (ptr2[j - 1][i - 1] != 0 ||
					ptr2[j - 1][i] != 0 ||
					ptr2[j - 1][i + 1] != 0 ||
					ptr2[j][i - 1] != 0 ||
					ptr2[j][i + 1] != 0 ||
					ptr2[j + 1][i - 1] != 0 ||
					ptr2[j + 1][i] != 0 ||
					ptr2[j + 1][i + 1] != 0)
				{
					ptr1[j][i] = 255;
				}
			}
		}
}
```
  
  
# 3. 본처리

## ROI 설정

- ROI(Region of Interest)는 영상에서 사용자가 관심있어 하는 영역
- 전체 이미지가 큰 경우 ROI를 설정함으로써 연산량을 감소 시킬때 주로 사용

![Roi 설정](https://sat02pap001files.storage.live.com/y4mMGN3ty0khhk35Q_KUA-W5TpR6FAYJF_4scdWKcc7aXMMZkz5id11am-MYsF-igTGmokdgwgNGzqLz6y3NRbNRTUU3_FyIjkvs1d-AYbWY7VnxJr4LMhi67s3cakCkGQOzxAJMmf9egc3hX8pdVTo7PoPMxoAtZXzx-m8-THA0w_U-Q-UtSQH-iVxqlwok1c3n1ffFDyqpVFVUcbGV0Y3JA/roi%EC%84%A4%EC%A0%95.png?psid=1&width=1000&height=750)

### 구현 코드

```c++
void ROI::ROIDibBinarization(MyImage& image, int th)
{
	register int i, j;
	BYTE** ptr = image.GetPtr();


	for (j = m_pos.y*4 ; j < m_dPos.y*4; j++)
		for (i = m_pos.x*4; i < m_dPos.x*4; i++)
		{
			ptr[j][i] = (ptr[j][i] > th) ? 255 : 0;
		}
}
```

## Hole 중심점 찾기

- 홀의 중심점을 찾고 영상의 중심(w/2, h/2)와의 차이를 구함

![홀 중심점](https://sat02pap001files.storage.live.com/y4miuiFihbPEm7ra1tVHz5WadktNs91fwfDHbetuOxIgrGQME5m9Mxn-kQlqdmgVpIlVs8wlIZdfDtzMkZNw73v5RRQ8bTfw6Fn35E2y9KTTd2otKeBweyxzgqC32j_Jlg_KeoNd4M2d9PP3SVdzZPSSUKiY-GC9JBzQKIhLBGjxX9OTuaomgQArj0EKGopqLxxpJFoKqjL-p1wIz4mVAZT6Q/%ED%99%80%EC%A4%91%EC%8B%AC%EC%A0%90.png?psid=1&width=1938&height=879)

||x|y|
|영상의중심|1296|972|
|Hole 중심점|1294|972|
|차이|+2|0|

### 구현 코드

```c++
CPoint ROI::FindHole(MyImage & image, CPoint x1, CPoint x2)
{
	BYTE** ptr = image.GetPtr();

	int n_roiCenterX;
	int n_roiCenterY;
	CPoint top, bottom, left, right;
	// ROI ÁßÁ¡
	n_roiCenterX = (x2.x + x1.x) / 2;
	n_roiCenterY = (x2.y + x1.y) / 2;

	int x, y;

	x = n_roiCenterX;
	y = n_roiCenterY;
	while (true) {
		if (ptr[y][x] == 0) {
			left.x = x;
			left.y = y;
			break;
		}
		x = x - 1;
		if (x == x1.x) {
			break;
		}
	}

	x = n_roiCenterX;
	y = n_roiCenterY;
	while (true) {
		if (ptr[y][x] == 0) {
			right.x = x;
			right.y = y;
			break;
		}
		x = x + 1;
		if (x == x2.x) {
			break;
		}
	}

	x = n_roiCenterX;
	y = n_roiCenterY;
	while (true) {
		if (ptr[y][x] == 0) {
			top.x = x;
			top.y = y;
			break;
		}
		y = y - 1;
		if (y == x1.y) {
			break;
		}
	}

	x = n_roiCenterX;
	y = n_roiCenterY;
	while (true) {
		if (ptr[y][x] == 0) {
			bottom.x = x;
			bottom.y = y;
			break;
		}
		y = y + 1;
		if (y == x2.y) {
			break;
		}
	}

	//CPoint pos1(left.x, top.y);
	//CPoint pos2(right.x, top.y);
	//CPoint pos3(right.x, bottom.y);
	//CPoint pos4(left.x, bottom.y);
	x = (left.x + right.x) / 2;
	y = (top.y + bottom.y) / 2;

	//return GetIntersectPoint2D(top, bottom, left, right);
	return CPoint(x, y);
}

```
  
  
# 4. 후처리

## 이동거리 구하기

- 삼각 함수를 이용하여 회전각, x축 이동거리, y축 이동거리 구하기

![회전각](https://sat02pap001files.storage.live.com/y4m6sUt7eHTTQxxHSGxC07o9xTIfpRCSq0wCSrYKPrg1V4kNSyDLDDhyTNc05CVORPi1h0pdf9wbddj3EUxgx7EI5uPo_ghVa9Pi8pJMzaNykaAEhTMwbSdxiLdcFR2citrpNmK6Fcr5gTmD2277KWs8hNOdCXh2sP71cWFT9yAYd28LLxuCUcDpH6J_NyZcijfCT0Yf-3NsAhhSH-UPUmPbw/%ED%9A%8C%EC%A0%84%EA%B0%81%20%EA%B5%AC%ED%95%98%EA%B8%B0.png?psid=1&width=1000&height=750)

- (eX, eY)가 ear영상의 홀 중심 좌표
- (hX, hY)가 home영상의 홀 중심 좌표
- (w/2, h/2)가 영상의 중심 좌표 => 홀중심과 영상의 중심의 차이를 구한다.

## 회전각 구하기

- 두 영상 홀중심의 길이가 빗변이 되고, 이 길이를 140000 µm 라고 가정한다. 
- α = hX – eX 
- β = hY - eY
- 경사 각 θ를 구하는 arctan 공식에 대입하여 경사각을 구한다.
- 최종식은 Θ = arctan(β, α) 된다

![회전각](https://sat02pap001files.storage.live.com/y4m7ltWl8R4MAu61CYYYa0Bw6jHuf8gpOBonhB7RpwaBjTY4ybEv_HY0xoATYb2B6aPuhF7eL1Zfma9aZ8ytv8UEXsqMJTDwE712rThs5KDNg2CxUHZ_ddwO_Mjd3M_wTh4tTvfWNv5LOloKW-LqZVdVq8mGnCOLt6Z6GRS8Vphnw_t6Zf9lWA5M9BigFE-hc1JIjFnnmmND00R07YekreuZQ/%ED%9A%8C%EC%A0%84%EA%B0%812.png?psid=1&width=527&height=547)


## tX, tY 구하기

- 경사 각 θ 와 반지름 r을 이용해 tX, tY 의 비율을 구한다.
- tX = center.x + cos(θ)*r
- tY = center.y + sin (θ)*r

> 이와 같은 방식으로 Film 영상의 오차를 구한 뒤 연산을 하여, Glass를 기준으로 Film이 이동해야 하는 거리와 회전각을 구한다.

![tx,ty](https://sat02pap001files.storage.live.com/y4mEt-rMiHFPwphn7koDSkrl3MUxFocRU53ah51An1jfD1vBgC9oeThBMp7t76PEO1SZYWh3dSr4FxfeQmP0ihGK_56ellCEywANY03rJWjVLG6Val9qdws5y2s2-Bt182Ouuiz4Z5JCG5IajSO_NSMD2drm92Q18uo_rKG3qrnrdwKcDTJirF8PQGwO3ASJsY8mQ3qIe4Taq4Uu8b-2__7Pg/tX%2CtY%20%EA%B5%AC%ED%95%98%EA%B8%B0.png?psid=1&width=421&height=385)

### 구현 코드

```c++
FLOAT CToolDlg::EvalX(CPoint pos1, CPoint pos2)
{
	float rad;
	float deg;

	int x = (pos2.x * 10) - (pos1.x * 10);
	int y = (pos2.y * 10) - (pos1.y * 10);

	rad = atan2(140000 + y, x);
	float dx = cos(rad)*sqrt(pow(x, 2) + pow(y + 140000, 2)) / 2;
	float dgx = 0 - ((pos1.x * 10) + dx);
	return dgx;
}

FLOAT CToolDlg::EvalY(CPoint pos1, CPoint pos2)
{
	float rad;
	float deg;

	int x = (pos2.x * 10) - (pos1.x * 10);
	int y = (pos2.y * 10) - (pos1.y * 10);

	rad = atan2(140000 + y, x);
	float dy = sin(rad)*sqrt(pow(x, 2) + pow(y + 140000, 2)) / 2;
	float dgy = dy - 70000 - (pos2.y * 10);
	return dgy;
}
```
  
  
# 5. 최종 결과물

- MFC 를 이용하여 GUI 및 드레그 드랍 이벤트 구현
- [전체 코드 GitHub](https://github.com/hayagou/VisionAlign)


![결과물](https://sat02pap001files.storage.live.com/y4mtvpBy-eaYwrbSgwJpdpqtqJUkTFufenMqlyZLoPKI46oROM6a-PHTUBI1Qi6GBbxFWT_vOk_3cAM3UfkobfsIz8kSVm_UX4rmMy4swFp1OlC1CznebJP_TOJO9KCkRgVi9xxmtas419WPMN9HWRrXvXi2QCjIzzoxkc6fPPMid-u32eW8LE30fqr_sw1dseiEeXiviwb4TS8Pw8j6tpheA/%EC%B5%9C%EC%A2%85%EA%B2%B0%EA%B3%BC%EB%AC%BC.png?psid=1&width=1000&height=750)
