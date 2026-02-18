/**
 ******************************************************************************
 * @file    sto_pll_stub.c
 * @brief   Minimal stub for STO_PLL — replaces sto_pll_speed_pos_fdbk.c.
 *
 *          The SMO fills in STO_PLL_M1._Super fields directly from mc_tasks_foc.c.
 *          This file provides only the empty function bodies that the linker
 *          expects so we can remove the full 8 kB sto_pll_speed_pos_fdbk.c.
 *
 *          Flash savings: ~8-10 kB (entire STO-PLL + PLL observer removed).
 ******************************************************************************
 */

#include "mc_config.h"
#include "speed_pos_fdbk.h"
#include <string.h>

/* ── Empty stubs for functions called from mc_tasks_foc.c ──────────────── */

void STO_PLL_Init(STO_PLL_Handle_t *pHandle)
{
    /* SMO is initialised in mc_tasks_foc.c instead.
     * Just zero the handle to be safe.                                      */
    (void)memset(pHandle, 0, sizeof(*pHandle));
    
    /* Initialise the base SpeednPosFdbk_Handle_t fields to sane defaults    */
    pHandle->_Super.hElAngle         = 0;
    pHandle->_Super.hMecAngle        = 0;
    pHandle->_Super.hAvrMecSpeedUnit = 0;
    pHandle->_Super.hElSpeedDpp      = 0;
    pHandle->_Super.hMecAccelUnitP   = 0;
    pHandle->_Super.bSpeedErrorNumber = 0;
    pHandle->_Super.bElToMecRatio    = (uint8_t)POLE_PAIR_NUM;
}

void STO_PLL_Clear(STO_PLL_Handle_t *pHandle)
{
    /* smo_reset() is called instead from mc_tasks_foc.c                     */
    pHandle->_Super.hElAngle         = 0;
    pHandle->_Super.hAvrMecSpeedUnit = 0;
    pHandle->_Super.hElSpeedDpp      = 0;
}

void STO_PLL_CalcAvrgElSpeedDpp(STO_PLL_Handle_t *pHandle)
{
    /* SMO updates hAvrMecSpeedUnit directly; nothing to do here.
     * This function is kept as a no-op for backwards compatibility with
     * code that calls it after the SMO update.                              */
    (void)pHandle;
}

void STO_SetDirection(STO_PLL_Handle_t *pHandle, int8_t direction)
{
    /* SMO infers direction from omega_est sign; no separate call needed.
     * This stub exists only so the linker is happy.                         */
    (void)pHandle;
    (void)direction;
}

bool STO_PLL_IsObserverConverged(STO_PLL_Handle_t *pHandle, int16_t *pMecSpeedUnit)
{
    /* REPLACED by smo_is_converged() in mc_tasks_foc.c.
     * If this function is somehow still called from revup_ctrl.c, return
     * false so it doesn't interfere with the SMO convergence logic.         */
    (void)pHandle;
    (void)pMecSpeedUnit;
    return false;
}

int16_t STO_PLL_CalcAvrgMecSpeedUnit(STO_PLL_Handle_t *pHandle, int16_t *pMecSpeedUnit)
{
    /* SMO writes hMecSpeedUnit directly; just return it.                    */
    *pMecSpeedUnit = pHandle->_Super.hAvrMecSpeedUnit;
    return *pMecSpeedUnit;
}

bool STO_PLL_CalcElAngle(STO_PLL_Handle_t *pHandle, Observer_Inputs_t *pInputs)
{
    /* REPLACED by smo_update() in mc_tasks_foc.c.
     * SMO writes hElAngle into _Super; this stub does nothing.              */
    (void)pHandle;
    (void)pInputs;
    return true; /* Always report success so state machine doesn't fault     */
}

/* ── If the linker complains about missing STO_Handle functions ─────────── */
/* Some ST SDK versions also reference plain STO (not STO_PLL) during revup.
 * Add these stubs only if you get linker errors about undefined STO_* symbols. */

#ifdef INCLUDE_STO_STUBS
void STO_Clear(STO_Handle_t *pHandle)
{
    (void)pHandle;
}

bool STO_CalcElAngle(STO_Handle_t *pHandle, Observer_Inputs_t *pInputs)
{
    (void)pHandle;
    (void)pInputs;
    return true;
}
#endif /* INCLUDE_STO_STUBS */
