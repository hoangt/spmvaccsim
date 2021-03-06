#ifndef MEMORYSYSTEM_H
#define MEMORYSYSTEM_H

#include <QString>
#include <QMap>
#include <systemc.h>
#include <stdint.h>
#include "utilities.h"
#include "DRAMSim2/DRAMSim.h"

class MemorySystem : public sc_module
{
    SC_HAS_PROCESS(MemorySystem);

public:
    MemorySystem(sc_module_name name);
    ~MemorySystem();

    void setRequestFIFO(sc_fifo<MemoryOperation *> * fifo);
    void setResponseFIFO(int originID, sc_fifo<MemoryOperation *> * fifo);

    bool canAddRequest();
    void addRequest(MemoryOperation * op);

    void runMemorySystem();

    void printFinalStats();

    // callbacks from DRAMsim
    void readComplete(unsigned id, uint64_t address, uint64_t clock_cycle);
    void writeComplete(unsigned id, uint64_t address, uint64_t clock_cycle);
    static void powerCallback(double background, double burst, double refresh, double actpre);

    // statistics
    double getAverageReqRespLatency();
    double getAveragePowerBackground();
    double getAveragePowerBurst();
    double getAveragePowerRefresh();
    double getAveragePowerActPre();

protected:
    QString m_systemConfigFile;
    int m_megsOfMemory;

    DRAMSim::MultiChannelMemorySystem * m_DRAMSim;
    DRAMSim::Callback<MemorySystem, void, unsigned, uint64_t, uint64_t> * m_readCB, * m_writeCB;
    sc_fifo<MemoryOperation *> * m_requests;
    QMap<int, sc_fifo<MemoryOperation *> * > m_responseFIFOs;
    QMap<DRAMSim::Transaction *, MemoryOperation *> m_reqsInFlight;

    uint64_t m_latencySamples, m_latencySum, m_dataVolumeSum;

    static double m_powerSum[4];
    static uint64_t m_powerSamples;


    // per-epoch statistics
    double m_numEpochSamples, m_epochTotalDataVolume;
    QMap<MemRequestTag, double> m_epochSamplesOfType;
    QMap<MemRequestTag, sc_time> m_epochLatencySamplesOfType;

    void addEpochSample(MemoryOperation * op);


};

#endif // MEMORYSYSTEM_H
