// Fill out your copyright notice in the Description page of Project Settings.


#include "CampeonatoData.h"

FCampeonatoData::FCampeonatoData(int32 n_times) : n_times(n_times), apenasIda(true)
{
    tabela.Init(FPosicaoData(), n_times);
    confrontos.AddUninitialized(n_times, n_times);
}

FCampeonatoData::FCampeonatoData(int32 n_times, bool apenasIda) : n_times(n_times), apenasIda(apenasIda)
{
    tabela.Init(FPosicaoData(), n_times);
    confrontos.AddUninitialized(n_times, n_times);
}

void FCampeonatoData::atualizaTabela(int32 indexX, int32 indexY, FResultadoData resultado)
{
    if (apenasIda)
        confrontos.casa[indexY].fora[indexX] = resultado;
    confrontos.casa[indexX].fora[indexY] = resultado;
}

void FCampeonatoData::ordenaTabela()
{
    tabela.StableSort(
        [this](const FPosicaoData& A, const FPosicaoData& B) // return true = A antes de B
        {
            if (A.pontos == B.pontos)
            {
                if (A.saldo_de_gols == B.saldo_de_gols)
                {
                    if (A.gols_pro == B.gols_pro)
                    {
                        int32 ptsA=0, ptsB=0;
                        int32 sgA=0, sgB=0;
                        int32 gpA, gpB;

                        gpA = confrontos.casa[A.index_time].fora[B.index_time].gols_casa +
                                confrontos.casa[B.index_time].fora[A.index_time].gols_fora;
                        gpB = confrontos.casa[A.index_time].fora[B.index_time].gols_fora +
                                confrontos.casa[B.index_time].fora[A.index_time].gols_casa;

                        sgA = gpA - gpB;
                        sgB = gpB - gpA;

                        if (confrontos.casa[A.index_time].fora[B.index_time].gols_casa >
                            confrontos.casa[A.index_time].fora[B.index_time].gols_fora)
                            ptsA += 3;
                        else if (confrontos.casa[A.index_time].fora[B.index_time].gols_fora >
                            confrontos.casa[A.index_time].fora[B.index_time].gols_casa)
                            ptsB += 3;
                        else
                        {
                            ptsA += 1;
                            ptsB += 1;
                        }

                        if (confrontos.casa[B.index_time].fora[A.index_time].gols_casa >
                            confrontos.casa[B.index_time].fora[A.index_time].gols_fora)
                            ptsB += 3;
                        else if (confrontos.casa[B.index_time].fora[A.index_time].gols_fora >
                            confrontos.casa[B.index_time].fora[A.index_time].gols_casa)
                            ptsA += 3;
                        else
                        {
                            ptsA += 1;
                            ptsB += 1;
                        }

                        if (ptsA == ptsB)
                        {
                            if (sgA == sgB)
                            {
                                if (gpA == gpB)
                                    return true;
                                else
                                    return gpA > gpB;
                            }
                            else
                                return sgA > sgB;
                        }
                        else
                            return ptsA > ptsB;

                    }
                    else
                        return A.gols_pro > B.gols_pro;
                }
                else
                    return A.saldo_de_gols > B.saldo_de_gols;
            }
            else
                return A.pontos > B.pontos;
        }
    );
}
