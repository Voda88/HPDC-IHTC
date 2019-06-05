// Helper function for calculating mean wall temperature, required by evohtc.c

real mean_wall_temp(Thread* t)
{
face_t f;
real A[ND_ND], Amag, mean;
real sumt, suma;
sumt = suma = 0.0;

begin_f_loop(f,t)
{
F_AREA(A,f,t);
Amag = NV_MAG(A);
sumt += F_T(f,t) * Amag;
suma += Amag;
}
end_f_loop(f,t);

mean = sumt/suma;
return mean;
}