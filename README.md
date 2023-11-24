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
#### Card System 사용법
- BP_ZCharacter - CardComponent
	- CardInventory (TArray)
 		-소지 중인 카드를 저장 
	- CardDeck (TArray)
		-전투에 돌입 시, 인벤토리에서 패시브 카드 등을 제외한 실제로 낼 수 있는 카드들을 저장 
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
		-덱을 무작위로 셔플함

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

