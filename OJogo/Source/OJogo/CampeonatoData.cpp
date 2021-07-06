// Fill out your copyright notice in the Description page of Project Settings.


#include "CampeonatoData.h"

FCampeonatoData::FCampeonatoData(int32 n_times) : 
    n_times(n_times), rodada_atual(0), apenasIda(true)
{
    tabela.Init(FPosicaoData(), n_times);
}

FCampeonatoData::FCampeonatoData(int32 n_times, bool apenasIda) : 
    n_times(n_times), rodada_atual(0), apenasIda(apenasIda)
{
    tabela.Init(FPosicaoData(), n_times);
}

void FCampeonatoData::adicionaTime(int32 index_time, int32 posicaoInicial)
{
    tabela[posicaoInicial].index_time = index_time;
    times.Add(index_time);
}

void FCampeonatoData::montaConfrontos()
{
    confrontos.AddUninitialized(times);

    FRodadaData rodadas;
    calendario.Init(rodadas, times.Num()-1);

    TArray<TArray<int32>> combinacoes;
    TArray<int32> combinacoesT;
    combinacoesT.Init(-1, times.Num());
    combinacoes.Init(combinacoesT, times.Num());
    
    TArray<TSet<int32>> timeOcupadoRodada;
    TSet<int32> vazio;
    timeOcupadoRodada.Init(vazio, times.Num());

    for (int32 t1 = 0; t1 < times.Num(); ++t1)
    {
        for (int32 t2 = t1+1; t2 < times.Num(); ++t2)
        {
            int32 rodadaEscolhida;
            for (int32 rodada = 0; rodada < times.Num()-1; ++rodada)
            {
                if (!timeOcupadoRodada[t1].Contains(rodada) && !timeOcupadoRodada[t2].Contains(rodada))
                {
                    rodadaEscolhida = rodada;
                    break;
                }
            }
            timeOcupadoRodada[t1].Add(rodadaEscolhida);
            timeOcupadoRodada[t2].Add(rodadaEscolhida);
            combinacoes[t1][t2] = rodadaEscolhida;
            calendario[rodadaEscolhida].jogos.Add(FJogoData(times[t1], times[t2]));
        }
    }
}

void FCampeonatoData::atualizaTabela(int32 index_casa, int32 index_fora, int32 gols_casa, int32 gols_fora)
{
    if (apenasIda)
        confrontos.casa[index_fora].fora[index_casa] = FResultadoData(gols_casa, gols_fora);
    confrontos.casa[index_casa].fora[index_fora] = FResultadoData(gols_casa, gols_fora);

    for (int32 posicao_index = 0; posicao_index < tabela.Num(); ++posicao_index)
    {
        if (tabela[posicao_index].index_time == index_casa)
        {
            tabela[posicao_index].pontos += (gols_casa > gols_fora)? 3 : (gols_casa == gols_fora)? 1 : 0;
            tabela[posicao_index].partidas_jogadas++;
            tabela[posicao_index].vitorias += (gols_casa > gols_fora)? 1 : 0;
            tabela[posicao_index].empates += (gols_casa == gols_fora)? 1 : 0;
            tabela[posicao_index].derrotas += (gols_casa < gols_fora)? 1 : 0;
            tabela[posicao_index].gols_pro += gols_casa;
            tabela[posicao_index].gols_contra += gols_fora;
            tabela[posicao_index].saldo_de_gols += (gols_casa - gols_fora);
        }
        else if (tabela[posicao_index].index_time == index_fora)
        {
            tabela[posicao_index].pontos += (gols_fora > gols_casa)? 3 : (gols_fora == gols_casa)? 1 : 0;
            tabela[posicao_index].partidas_jogadas++;
            tabela[posicao_index].vitorias += (gols_fora > gols_casa)? 1 : 0;
            tabela[posicao_index].empates += (gols_fora == gols_casa)? 1 : 0;
            tabela[posicao_index].derrotas += (gols_fora < gols_casa)? 1 : 0;
            tabela[posicao_index].gols_pro += gols_fora;
            tabela[posicao_index].gols_contra += gols_casa;
            tabela[posicao_index].saldo_de_gols += (gols_fora - gols_casa);
        }
    }
}

void FCampeonatoData::terminaRodada()
{
    rodada_atual += 1;
    
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
                                    return FMath::RandBool();
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
