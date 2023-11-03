#pragma once

void InitPatchs();

void ReduceCPU();

void ReduceRam(LPVOID lpThreadParameter);

void FixShopNpcClose();

void IgnoreRandomStuck(DWORD c, DWORD Damage);

void FixChasingAttackMovement();

void DecBMD();

int OpenTerrainMapping(char* FileName);

int OpenTerrainAttribute(char* FileName);

int OpenObjectsEnc(char* FileName);