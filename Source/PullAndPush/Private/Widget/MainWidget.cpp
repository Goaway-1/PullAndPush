#include "Widget/MainWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UMainWidget::NativeConstruct()
{	
	HostButton->OnClicked.AddDynamic(this, &UMainWidget::OnHostClicked);
	JoinButton->OnClicked.AddDynamic(this, &UMainWidget::OnJoinClicked);
	HostText->SetText(FText::FromString("Host"));
	JoinText->SetText(FText::FromString("Join"));
}
void UMainWidget::OnHostClicked()
{
	// @TODO : 호스팅 작업
	PPLOG(Warning, TEXT("OnHostClicked"));

}
void UMainWidget::OnJoinClicked()
{
	// @TODO : 참여 작업
	PPLOG(Warning, TEXT("OnJoinClicked"));

}