function dx = sf_bioc_model(~,x,u)

V   = x(1);
cX  = x(2);
cP  = x(3);
cS  = x(4);
cO2 = x(5);
T   = x(6);
Tag = x(7);

Fi    = u(1);
Fe    = u(2);
T_in  = u(3);
cS_in = u(4);
Fag   = u(5);

HNa  = -0.550; HCa  = -0.303; HMg  = -0.314;
HH   = -0.774; HCl  =  0.844; HCO3 =  0.485; HHO  =  0.941;

MNaCl=58.5; MCaCO3=90; MMgCl2=95;
MNa=23; MCa=40; MMg=24; MCl=35.5; MCO3=60;

miu_P=1.790; Ks=1.030; Ks1=1.680;
Kp=0.139; Kp1=0.070; Rsx=0.607; Rsp=0.435;
YO2=0.970; KO2=8.86; miu_O2=0.5;
A1=9.5e8; A2=2.55e33; Ea1=55000; Ea2=220000; R=8.31;

Kla0=38; KT=100*3600; Vm=50; AT=1;
ro=1080; ccal=4.18; roag=1000; ccalag=4.18;
deltaH=518;

mNaCl=500; mCaCO3=100; mMgCl2=100;
pH=6; Tiag=15;

c0st = 14.16 - 0.3943*T + 0.007714*T^2 - 0.0000646*T^3;

cNa  = mNaCl/MNaCl*MNa/V;
cCa  = mCaCO3/MCaCO3*MCa/V;
cMg  = mMgCl2/MMgCl2*MMg/V;
cCl  = (mNaCl/MNaCl + 2*mMgCl2/MMgCl2)*MCl/V;
cCO3 = mCaCO3/MCaCO3*MCO3/V;
cH   = 10^(-pH);
cOH  = 10^(-(14-pH));

INa  = 0.5*cNa;
ICa  = 0.5*cCa*4;
IMg  = 0.5*cMg*4;
ICl  = 0.5*cCl;
ICO3 = 0.5*cCO3*4;
IH   = 0.5*cH;
IOH  = 0.5*cOH;

sumaHiIi = HNa*INa+HCa*ICa+HMg*IMg+HCl*ICl+HCO3*ICO3+HH*IH+HHO*IOH;
cst  = c0st * 10^(-sumaHiIi);

Kla  = Kla0*(1.024^(T-20));

rO2 = miu_O2 * cO2 * cX/YO2/(KO2 + cO2)*1000;

miu_X = A1*exp(-Ea1/R/(T+273)) - A2*exp(-Ea2/R/(T+273));

dV   = Fi - Fe;
dcX  = miu_X*cX*cS/(Ks+cS)*exp(-Kp*cP) - (Fe/V)*cX;
dcP  = miu_P*cX*cS/(Ks1+cS)*exp(-Kp1*cP) - (Fe/V)*cP;
dcS  = -miu_X*cX*cS/(Ks+cS)*exp(-Kp*cP)/Rsx ...
       -miu_P*cX*cS/(Ks1+cS)*exp(-Kp1*cP)/Rsp ...
       +(Fi/V)*cS_in - (Fe/V)*cS;
dcO2 = Kla*(cst-cO2) - rO2 - (Fe/V)*cO2;
dT   = (1/32*V*rO2*deltaH - KT*AT*(T-Tag) ...
       +Fi*ro*ccal*(T_in+273) - Fe*ro*ccal*(T+273))/(ro*ccal*V);
dTag = (Fag*ccalag*roag*(Tiag-Tag) + KT*AT*(T-Tag)) ...
       /(Vm*roag*ccalag);

dx = [dV; dcX; dcP; dcS; dcO2; dT; dTag];
end