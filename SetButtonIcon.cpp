void UWLBattleview_ControlField::SetAttackButtonIcon(bool IsForce /*= false*/)
{
    if (ListItem_AutoAttack == nullptr || ListItem_AutoAttack->IsValidLowLevel() == false)
        return;

    if(ListItem_AutoAttack->GetActionType() == EInputKeyType::Attack && IsForce == false)
        return;

    if (m_OwnCharacter == nullptr || m_OwnCharacter->IsValidLowLevel() == false)
        return;

    const FWLCharacterData* CharacterData = m_OwnCharacter->GetCharacterData<FWLCharacterData>();
    if(CharacterData == nullptr)
        return;

    UWLTable* Table = GameInstance->GetInstance<UWLTable>();
    if(Table == nullptr || Table->IsValidLowLevel() == false)
        return;
        
    FWLSkillSetData* SkillSetData = Table->GetSkillSetDataAt(CharacterData->skill_set_idx);
    if(SkillSetData.attack_list.Num() == 0)
        return;

    int32 AttackTableID = SkillSetData.attack_list[0];
    
    FWLSkillData SkillData;
    if(Table->GetSkillDataAt(AttackTableID, SkillData) == true)
    {
        FWLIconCode IconTable = Table->GetIconCodeDataAt(SkillData.icon);
        IconTable.icon_path.LoadSynchronous();

        ListItem_AutoAttack->SetActionType(EInputKeyType::Attack);
        ListItem_AutoAttack->SetButtonIconEvent(IconTable.icon_path.Get());
    }
}
