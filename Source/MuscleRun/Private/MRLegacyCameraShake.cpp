// Fill out your copyright notice in the Description page of Project Settings.


#include "MRLegacyCameraShake.h"

UMRLegacyCameraShake::UMRLegacyCameraShake()
{
	// 회전 진동 (Pitch, Yaw, Roll) 카메라의 회전을 흔들기 (Pitch: 위아래, Yaw: 좌우, Roll: 기울기)
	RotOscillation.Pitch.Amplitude = 3.0f; //	흔들리는 강도
	RotOscillation.Pitch.Frequency = 25.0f; // 흔들리는 속도 (1초에 몇 번 왔다 갔다)

	RotOscillation.Yaw.Amplitude = 3.0f;
	RotOscillation.Yaw.Frequency = 25.0f;

	RotOscillation.Roll.Amplitude = 3.0f;
	RotOscillation.Roll.Frequency = 25.0f;
	 
	// 위치 진동 (X, Y, Z) 	카메라의 위치를 흔들기 (X, Y, Z 방향)
	LocOscillation.X.Amplitude = 5.0f;
	LocOscillation.X.Frequency = 20.0f;

	LocOscillation.Y.Amplitude = 5.0f;
	LocOscillation.Y.Frequency = 20.0f;

	LocOscillation.Z.Amplitude = 5.0f;
	LocOscillation.Z.Frequency = 20.0f;

	// 지속 시간 및 블렌딩 , 시작과 끝을 부드럽게 만드는 시간
	OscillationDuration = 0.5f;  // 전체 흔들리는 시간, 흔들림이 0.5초 지속
	OscillationBlendInTime = 0.1f;  // 시작할 때 0.1초 점점 강해짐
	OscillationBlendOutTime = 0.2f;  // 끝날 때 0.2초 점점 약해짐

	// 커스텀 프로퍼티 (원한다면 UI에서 조정 가능)
	MyCustomShakeAmplitude = 10.0f;   // 사용자는 이 값으로 진동 세기 조절 가능 (아직 코드에는 반영 안됨)
}
 
/* 전체적인 흐름 요약
 UMRLegacyCameraShake 클래스를 만들어서 진동 패턴을 정의한다.

캐릭터(AMRPlayerCharacter)가 어떤 이벤트에서 PlayCameraShake()를 호출한다.

그 안에서 GetController()로 플레이어 컨트롤러를 찾고 ClientStartCameraShake()을 호출한다.

언리얼 엔진이 자동으로 화면을 흔들리는 효과를 적용한다.

응용 팁(추후에 추가할 수 있음)
조건부 셰이크 : 데미지를 많이 받을수록 더 흔들리게 만들기

셰이크 중복 방지 : 일정 시간 안에 다시 실행 안 되게 Cooldown 설정

사운드나 VFX와 함께 실행 : 더 실감나는 연출 가능*/