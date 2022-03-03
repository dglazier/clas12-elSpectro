echo 'Checking    CLAS12_Simple_Flat_s.C  I'
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc "--baryon=2212: --meson=221:111,211,-211" CLAS12_Simple_Flat_s.C
echo 'Checking    CLAS12_Simple_Flat_s.C  II'
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--baryon=3122:2212,-211 --meson=321' CLAS12_Simple_Flat_s.C
echo 'Checking   CLAS12_ep_pJpsi_t_s.C I'
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--nucleon=2212  --tslope=4 --flat=1 --muons' CLAS12_ep_pJpsi_t_s.C
echo 'Checking   CLAS12_ep_pJpsi_t_s.C II'
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--nucleon=2212  --tslope=4 --flat=1 --jpac=3' CLAS12_ep_pJpsi_t_s.C
echo 'Checking    MesonEx_DeltaEtaPi_Flat_s_give_t_MEtaPi.C '
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--tslope=4 --flat=1 --MEtaPi=0.9*TMath::BreitWigner(x,0.78,0.149)' MesonEx_DeltaEtaPi_Flat_s_give_t_MEtaPi.C
echo 'Checking   MesonEx_DeltaEtaPi_Flat_s_give_t_MEtaPi.C '
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--tslope=4 --flat=1' MesonEx_DeltaEtaPi_Flat_s_give_t_MEtaPi.C
echo 'Checking   MesonEx_n3pi_Flat_s_give_t_M3pi.C '
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--M2pi=TMath::BreitWigner(x,0.78,0.149) --M3pi=TMath::BreitWigner(x,1.5,0.3) --tslope=4 --flat=1' MesonEx_n3pi_Flat_s_give_t_M3pi.C
echo 'Checking    MesonEx_p2K_Flat_s_give_t_M2K.C'
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--M2K=TMath::BreitWigner(x,0.78,0.149) --tslope=4 --flat=1' MesonEx_p2K_Flat_s_give_t_M2K.C
echo 'Checking    MesonEx_p2pi_Flat_s_give_t_M2pi.C'
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--tslope=4 --flat=1' MesonEx_p2pi_Flat_s_give_t_M2pi.C
echo 'Checking    VeryStrange_KKXi_Flat_s_give_t_MXi.C'
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--tslope=4 --flat=1' VeryStrange_KKXi_Flat_s_give_t_MXi.C
echo 'Checking    CLAS12_eD_NJpsi_t_s.C'
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--nucleon=2212 --tslope=4 --flat=1 --muons' CLAS12_eD_NJpsi_t_s.C
echo 'Checking    VeryStrange_KpKpXi_KmY_Flat_MXi.C'
clas12-elSpectro --ft --ebeam 10.6 --seed 2132 --trig 1000 --misc '--MXi=TMath::BreitWigner(x,1.823,0.024) --Hyperon=3122' VeryStrange_KpKpXi_KmY_Flat_MXi.C
echo 'Checking    CLAS12_pEta_Flat_s.C'
clas12-elSpectro --ft --ebeam 10.6 --seed 2132 --trig 1000 CLAS12_pEta_Flat_s.C
echo 'Checking    CLAS12_eD_npi0_Flat_s.C'
clas12-elSpectro  --ebeam 10.6 --seed 2132 --trig 1000 CLAS12_eD_npi0_Flat_s.C
echo 'Checking    MesonEx_nKKPi_Flat_s_give_t_MKKPi.C'
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--tslope=4 --flat=1' MesonEx_nKKPi_Flat_s_give_t_MKKPi.C
echo 'Checking    MesonEx_nKPiK_Flat_s_give_t_MKPiK.C'
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--tslope=4 --flat=1' MesonEx_nKPiK_Flat_s_give_t_MKPiK.C
echo 'Checking    MesonEx_nKstarK_Flat_s_give_t_MKPiK.C'
clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 1000 --misc '--MKmPi=TMath::BreitWigner(x,0.892,0.026) --tslope=4 --flat=1' MesonEx_nKPiK_Flat_s_give_t_MKPiK.C
echo 'Checking     CLAS24_JPAC_nZc.C'
clas12-elSpectro --ebeam 22 --seed 2132 --trig 1000 --misc '--tslope=4 --flat=1 --muons  --jpac=1' CLAS24_JPAC_nZc.C
echo 'Checking     CLAS24_JPAC_pX.C'
clas12-elSpectro --ebeam 22 --seed 2132 --trig 1000 --misc '--tslope=4 --flat=1 --muons  --jpac=1' CLAS24_JPAC_pX.C
