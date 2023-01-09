void UBattleview_ControlField::SetAttackButtonIcon(bool IsForce /*= false*/)
{
    if (Bt_Attack == nullptr || Bt_Attack->IsValidLowLevel() == false)
        return;

    if (Bt_Attack->GetInputType() == EInputType::Attack && IsForce == false)
        return;

    if (m_OwnCharacter == nullptr || m_OwnCharacter->IsValidLowLevel() == false)
        return;

    const FCharacterData* CharacterData = m_OwnCharacter->GetCharacterData<FCharacterData>();
    if (CharacterData == nullptr)
        return;

    UTable* Table = GameInstance->GetInstance<UTable>();
    if (Table == nullptr || Table->IsValidLowLevel() == false)
        return;

    FSkillSetData SkillSetData = Table->GetSkillSetDataAt(CharacterData->skill_set_idx);
    
    int32 AttackTableID = SkillSetData.attack_list[0];
    FSkillData SkillData = Table->GetSkillDataAt(AttackTableID);
    
    FIconCode IconTable = Table->GetIconCodeDataAt(SkillData.icon);
    IconTable.icon_path.LoadSynchronous();

    Bt_Attack->SetInputType(EInputType::Attack);
    Bt_Attack->SetButtonIconEvent(IconTable.icon_path.Get());
}
