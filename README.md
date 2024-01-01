# ProjectZ
ProjectZ

---
### 개발팀 규칙

1. 버전은 UE 5.1.1을 사용한다.
2. 담당은 비전투씬, 전투씬, UI/UX/데이터 저장으로 나뉘며, 순서대로 각 담당은 승민님, 재균님, 규현님 및 성일님으로 지정된다.
3. Convention은 항상 CamelCase를 사용한다.
4. **!!절대로 git add . 을 사용하지 말 것!!**
5. **!!절대로 main branch에 pull request 없이 merge 하지 말 것!!**
---
다들 화이팅입니다!
---
#### Ability System 사용법
- 기능이 늘어갈때마다 추가 예정
- [공식 문서](https://docs.unrealengine.com/5.0/ko/gameplay-ability-system-for-unreal-engine/)
- AttributeSet : 각종 능력치들을 정의하는 Class
	- [공식 문서](https://docs.unrealengine.com/5.2/ko/gameplay-attributes-and-attribute-sets-for-the-gameplay-ability-system-in-unreal-engine/)
	- 능력치의 변수 Type : FGameplayAttributeData
	- ATTRIBUTE_ACCESSORS(UZAttributeSet, 변수명) : 능력치들의 Getter, Setter를 만들어주는 Macro
- GameplayTag
	- [공식 문서](https://docs.unrealengine.com/4.27/ko/ProgrammingAndScripting/Tags/)
	- 에디터에서 Project Setting - GameplayTags 에서 추가 (C++ 사용불가)
	- ZGameplayTag 클래스에서 추가 (C++ 사용가능)
	- Ex) 체력 포션을 만들 때 : Attribute.Vital.Health 태그를 넣기
	- Ex) 스턴 능력이 있는 공격 : Debuff.Stun 태그를 넣기
	- Ex) 독 공격을 받았지만 독 면역이 있는 경우 : 공격에 Debuff.Poison 태그가 있으면, 그 공격을 무효로 한다
	- Ex) 현재 적용된 버프, 디버프를 UI에 나타낼 때, Tag에 따라 아이콘을 변경한다
- GameplayEffect : 버프, 디버프, 데미지 등 캐릭터에 작용하는 여러 Effect, Only Blueprint
	- [공식 문서](https://docs.unrealengine.com/5.2/ko/gameplay-attributes-and-gameplay-effects-for-the-gameplay-ability-system-in-unreal-engine/)
	- Duration Policy
		- Instant : 즉시 작용하는 Effect. Ex) 체력 포션
		- Infinite : 지속되는 Effect. 따로 제거하는 코드를 만들어줘야함. Ex) 디버프, 버프
		- Has Duration : 지속 시간이 있는 Effect. Ex) 출혈, 독
	- Modifiers
		- Attribute : 변경하고 싶은 Attribute
		- Modifier Op : 더하기, 곱하기, 나누기, Override 등 Effect가 작용하는 방식
		- Modifier Magnitude
			- Magnitude Calculation Type
				- Scalable Float : 실수
				- Attribute Based : Coefficient * (PreMultiplyAdditiveValue +Attribute Value) + PostMultiplyAdditiveValue
				- Custom Caclulation Class : 더 복잡한 계산 방식이 필요 할 경우 GameplayModMagnitudeCalculation 클래스를 만들어서 사용 Ex) Attribute 가 여러개 필요 할 경우 (MMC_MaxHealth)
				- Set by Caller : Tag를 통해 Value를 설정
	- Tags : Effect에 Tag 넣기. 옵션들은 읽어보시는게 빠를거같아요
	- Stacking : Stacking Effect. Ex) 출혈 상태 이상 중첩 시키기
- Initialize Character Attribute
	- John Doe 만 예시로 제작
	- GE_InitializeJhonDoe : John Doe 가 기본적으로 가지고 있는 스탯들 (반사회성, 엔지니어링 등)
	- GE_SecondaryAttribute : 기본 스탯들로 계산되는 Attribute (MaxHealth, MaxMentality, 기획에 따라 크리티컬 확률 등등 추가)
	- GE_VitalAttribute : 현재 체력, 정신력
- Gameplay Ability
	- [공식 문서](https://docs.unrealengine.com/5.0/ko/using-gameplay-abilities-in-unreal-engine/)
	- 공격, 스킬, 능력 등을 구현
	- Gameplay Ability 를 부모 Class로 Blueprint 제작
	- Active Ability 함수를 Overide해서 Ability가 실행할 작업들을 수행 (Play Montage, Spawn Actor 등등)
	- 공격을 구현할 때 공격모션을 Animation Montage로 구현시 Ability가 Active 되는 타이밍을 Anim Notify로 설정 (애니메이션에서 돌을 던지는 타이밍을 설정)
	- GameplayAbility 에서는 ActivateAbility 이벤트에 PlayMontageAndWait 함수 뒤 WaitGameplayEvent 함수
	- Anim Notify는 AN_MontageEvent를 선택후 Event Tag를 설정
	- Anim Notify가 실행되면 설정한 Event Tag와 일치 할 경우 WaitGameplayEvent함수가 실행됨
	- Event Received 노드에 Ability가 할 일을 지정
	- GA_ThrowingStone을 예시로 만들었음 (NonCombat 모드에서 T키 를 눌러 사용)
		- Tags
			- Ability Tags : 능력이 가지고 있는 태그
			- Cancel Abilities with Tag : 특정 태그를 가지고 있는 능력을 취소
			- Block Abilities with Tag : 능력이 활성화 되어 있는 동안 특정 태그를 가지고 있는 능력 발동 불가
			- Activation Owned Tags : 능력이 활성화되는 동안 소유자에게 적용할 태그
			- Activation Required Tags : 태그들을 모두 가지고 있는 액터/컴포넌트만 이 능력을 발동 할 수 있음
			- Activation Blocked Tags : 태그들중 하나라도 가지고 있는 액터/컴포넌트는 이 능력을 발동 할 수 없음
			- Source Required Tags : Source가 태그들을 모두 가지고 있어야함
			- Source Blocked Tags : Source가 태그들을 하나라도 가지고 있지 않아야함
			- Target Required Tags : Target이 태그들을 모두 가지고 있어야함
			- Target Blocked Tags : Target이 태그들을 하나라도 가지고 있지 않아야함
		- Instancing Policy
			- Instanced Per Actor : 한 능력에 하나의 Instance 생성. 발동마다 재사용됨
				- Persistent한 data를 저장가능. 변수를 발동마다 재설정해야하는 경우 직접 해야됨 (Instance가 사라지지 않음)
			- Instanced Per Execution : 능력 발동마다 새로운 Instance를 생성
				- Persistent한 data 저장 불가능. Less Performace than Instanced Per Actor
			- Non-Instanced : Instance가 생성되지 않음
				- Best Performance. 할 수 있는 것들이 제한됨 (Static Function과 비슷)
		- Costs : Gameplay Effect로 능력을 사용하기 위한 Cost를 지불
		- Ability Triggers : Tag를 이용해 능력을 Activate
			- [참고 자료](https://onecoke.tistory.com/entry/Unreal-AbilityTriggers-in-GameplayAbility)
		- Cooldowns : Gameplay Effect로 능력의 쿨타임을 정함
- Gameplay Task
	- [공식 문서](https://docs.unrealengine.com/5.2/ko/gameplay-ability-tasks-in-unreal-engine/)
- ZInputComponent : 입력에 Tag를 넣어야하는 경우 DA_InputConfig 블루프린트에서 Input Action과 Tag를 넣을 수 있음
	- Enhanced Input을 상속 받아 만들었고, Project Setting에 기본 Input Component를 ZInputComponent로 변경

#### Card System 사용법
- Test용 기능 : 키보드 E 를 누르면 덱에서 손에 카드 한장을 추가 (나중에 전투 시스템 완성시 삭제)
- BP_ZCharacter - CardComponent
	- CardInventory (TArray)
		- 소지 중인 카드를 저장 
	- CardDeck (TArray)
		- 전투에 돌입 시, 인벤토리에서 패시브 카드 등을 제외한 실제로 낼 수 있는 카드들을 저장 
	- CardHand (TArray)
		- 전투 중 덱에서 뽑아 손에 있는 카드를 저장
	- FirstCardCount (int32)
		- 전투 돌입 시 첫 패로 뽑는 카드의 수
	- DrawAndAddCardDelegate 
		- UI에게 카드를 덱에서 뽑고 핸드에 추가하도록 하는 Delgate
	- AddCard(FName NewCard) 
		- 덱에 NewCard 카드를 추가함
	- DeleteCard(FName DeleteCard) 
		- 덱에서 DelteCard 카드를 제거함
	- ShuffleDeck()
		- 덱을 무작위로 셔플함

- WBP_CardHandWidget
	- HandCard (TArray)
		- 패에 있는 카드의 Widget을 저장 
	- LeftHandCard (FCard)
		- 왼쪽에 장착한 카드 
	- RightHandCard (FCard)
		- 오른쪽에 장착한 카드 
	- CardDistance (float)
		- 카드 사이의 거리 
	- CardAngle (float)
		- 손에 있는 카드 사이의 각도 
	- MouseHoveredHeight (float)
		- 마우스를 카드에 올렸을 때 올라가는 높이 
	- ArcHeight (float)
		- 카드 사이의 높이 
	- PlayCardHeight (float)
		- 카드를 내는 높이 
	- CardSize (FVector2D)
		- 카드의 크기 
	- CardSpawnPosition (int32 Index)
		- 카드의 스폰 위치
	- GetCardIndexPositionFromCenter(int32 Index) const
		- 가운데를 중심으로 Index를 -1, 0, 1 이런식으로 변환
	- CalculateCardPosition(int32 Index) 
		- Index를 받아 카드의 위치를 계산
	- AddCardToHand(FCard NewCard)
		- NewCard를 Hand에 추가, CardComponent의 DrawAndAddCardDelegate에 바인딩 되어있음
	- UpdateCardPosition()
		- HandCard 안에 있는 카드 위치를 지정

- WBP_CardWidget
	- CardName, CardImage, CardDescription, ManaText, AtkText, DefText
		- Widget에 바인딩 된 변수
	- CardDataTable (UDataTable)
		- CardName을 받고, DataTable에서 Data를 꺼내 카드를 만듬
	- InterpSpeed (int32)
		- 카드가 제 위치로 가는 속도
	- SetPosition(float DeltaTime)
		- 카드의 위치, 각도를 Interpolation으로 설정

