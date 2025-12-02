```c
#include "calcEcuTemp.h"

#include <stdbool.h> /* for bool */

/* Implementation of the function to compute ECU temperature estimate following MISRA rules. */

sint16 Calc_ecuTemperature_s16(void)
{
    /* Read inputs */
    extern sint16 NtcTemp_s16; /* NTC temperature in °C */
    extern sint16 SysTemp_s16; /* System temperature in °C */
    extern bool NtcErr_b;      /* NTC error flag */

    /* Local variable to store the computed ECU temperature estimate */
    sint16 l_ecuTemp_s16;

    /* Local variables for calculations */
    int32_t numerator;     /* To hold numerator for interpolation */
    int32_t denominator;   /* To hold denominator for interpolation */

    /* Initial assignment to prevent uninitialized use */
    l_ecuTemp_s16 = 0;

    /* Check for NTC error or low temperature condition */
    if ((NtcErr_b == true) || (NtcTemp_s16 <= PAR_ECU_TEMP_LOW_THR))
    {
        /* Use system temperature if NTC is faulty or at low temps */
        l_ecuTemp_s16 = SysTemp_s16;
    }
    else if (NtcTemp_s16 >= PAR_ECU_TEMP_UP_THR)
    {
        /* Use NTC temperature at high temps */
        l_ecuTemp_s16 = NtcTemp_s16;
    }
    else
    {
        /* Perform linear interpolation in the middle range */
        /* Compute numerator: (NtcTemp_s16 * (NtcTemp_s16 - low_thr) + SysTemp_s16 * (up_thr - NtcTemp_s16)) */
        numerator = (int32_t)NtcTemp_s16 * ((int32_t)NtcTemp_s16 - (int32_t)PAR_ECU_TEMP_LOW_THR)
                  + (int32_t)SysTemp_s16 * ((int32_t)PAR_ECU_TEMP_UP_THR - (int32_t)NtcTemp_s16);
        /* Compute denominator: (up_thr - low_thr) */
        denominator = (int32_t)(PAR_ECU_TEMP_UP_THR - PAR_ECU_TEMP_LOW_THR);

        /* Calculate interpolated temperature and assign */
        /* Use integer division; result in sint16, with potential rounding down */
        l_ecuTemp_s16 = (sint16)(numerator / denominator);
    }

    return l_ecuTemp_s16;
}
```