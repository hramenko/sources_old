//===========================================================================
#ifndef M_Dst_C0_C1tH
#define M_Dst_C0_C1tH
//===========================================================================
#ifndef mpiElementH
#include "mpiElement.h"
#endif
//===========================================================================

class TDst_C0_C1t : public TElement {
private:
        TFloat NEdit;

        int    N;
        int    N1, N2;
        TFloat Gamma;
        TFloat ro;
        bool   ChangingCoef;
        bool   Ro_Equal_One;

        TFloat  alfa1, alfa2, alfa3, Sigma0, Sigma1, a1_, a2_, a3_;
        TFloat  a0_c0,  a1_c0,  a2_c0,  a3_c0;
        TFloat  a0_c1,  a1_c1,  a2_c1,  a3_c1;
        int     CountInput;
        int     CurrentN;

        void    CalcCoef(int aN);
public:
        TDst_C0_C1t();

        virtual void     SaveToStream(TStream *aStream);
        virtual void     LoadFromStream(TStream *aStream);
        virtual void     DrawElementFace();
        virtual void     Edit();
        virtual void     Copy(void *aElement);
        virtual void     Init();
        virtual void     InitBeforeRun(){};
        virtual void     Run();
        virtual void     DoneRun(){};
        static TDst_C0_C1t *Create() { return new TDst_C0_C1t(); };
};
//===========================================================================
#endif
