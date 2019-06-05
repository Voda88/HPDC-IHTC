// UDF for calculating initial IHTC value before simulation
// AUTHOR: Mikko Karkkainen

DEFINE_ON_DEMAND(ihtc)
{
real PI = RP_Get_Real("pi");
real v = RP_Get_Real("v");	// cast metal impact velocity
real wave_speed = RP_Get_Real("wave_speed"); // sonic wave speed melt
real rho = RP_Get_Real("rho");	// cast metal density
real impact_angle = RP_Get_Real("impact_angle"); // Impact angle in degrees
real k_m = RP_Get_Real("k_m"); // mould thermal conductivity
real k_c = RP_Get_Real("k_c"); // cast metal thermal conductivity
real r_sm = RP_Get_Real("r_sm"); // Mean peak spacing
real sigma = RP_Get_Real("sigma"); // Mean peak height deviation
real epsilon = RP_Get_Real("epsilon"); // numerical parameter
real p_0 = RP_Get_Real("p_0"); // NTP pressure
real T_1 = RP_Get_Real("t_1"); // impact temperature
real T_0 = RP_Get_Real("t_0"); // NTP temperature

real y; // air gap thickness
real p_1; // water hammer pressure
real alpha; // helper variable
real lambda; // Effective thermal conductivity
real n; // mean of density of microcontact points
real a; // mean of contact point area
real h; // htc
real dx; // thickness of wall layer

FILE* fp;
Domain *d = Get_Domain(1);	// Calculation domain
Thread *t1,*t0;	
int zone_id = RP_Get_Real("wall_id"); 			// wall zone id (defined in initial_htc.scm)
int shadow_zone_id = RP_Get_Real("shadow_wall_id");;	// shadow wall zone id (defined in initial_htc.scm)
t1 = Lookup_Thread(d,zone_id); // Determine face thread for wall
t0 = Lookup_Thread(d,shadow_zone_id); // Determine face thread for shadow wall

// Calculate wall thickness based on corrected Hamasaiid model
lambda = 2*k_m*k_c/(k_m+k_c);
p_1 =  v*wave_speed*rho*sin(impact_angle);
y = pow(2.0,1.0/2.0)/pow(double(PI),1.0/6.0)*pow(double(p_0*T_1/(p_1*T_0)),1.0/3.0)*sigma;
alpha = y/(pow(2,1.0/2.0)*sigma);
n = 8*(1-erf(alpha))/(pow(PI,2)*epsilon*pow(r_sm,2));	
a = 1.0/4.0*r_sm*(2*exp(-pow(double(alpha),2.0))+(pow(double(2*PI),1.0/2.0)*y*(-1+erf(alpha))/sigma));
h = 2*lambda*a*n/pow(double(1-2*a/r_sm),3.0/2.0);
dx = RP_Get_Real("k_res")/h;
RP_Set_Real("y_0", y);
RP_Set_Real("thickness", dx);

// Print results in file output.txt
fp = fopen("output.txt","a");
fprintf(fp,"flow-time fs q h\n")
fprintf(fp,"%g %g %g %g\n", RP_Get_Real("flow-time"), 0, 0, h);
fclose(fp);
}