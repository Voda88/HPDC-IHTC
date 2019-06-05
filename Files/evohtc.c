// UDF for calculating new IHTC value after each time step
// AUTHOR: Mikko Karkkainen

DEFINE_ON_DEMAND(evolve_htc)
{

real PI = RP_Get_Real("pi");
real v = RP_Get_Real("v");	// cast metal impact velocity
real wave_speed = RP_Get_Real("wave_speed"); // sonic wave speed in H13 steel mould
real rho = RP_Get_Real("rho");	// cast metal density
real impact_angle = RP_Get_Real("impact_angle"); // Impact angle in degrees
real k_m = RP_Get_Real("k_m"); // mould thermal conductivity
real k_c = RP_Get_Real("k_c"); // cast metal thermal conductivity
real r_sm = RP_Get_Real("r_sm"); // Mean peak spacing
real sigma = RP_Get_Real("sigma"); // Mean peak height deviation
real epsilon = RP_Get_Real("epsilon"); // numerical parameter
real p_0 = RP_Get_Real("p_0"); // NTP pressure
real T_1 = RP_Get_Real("t_1"); // Melt impact temperature
real T_0 = RP_Get_Real("t_0"); // NTP temperature

real dx;				// wall thickness value corresponding thermal resistance
real temp1;				// mean wall temperature via mean_temp function
real temp2;				// mean shadow wall temperature via mean_temp function
real f_s = 0;			// fraction of solid

FILE *fp;				// output file variable

real p_1; // water hammer pressure
real alpha; // helper variable
real lambda; // Effective thermal conductivity
real n; // mean of density of microcontact points
real a; // mean of contact point area
real h; // htc

Domain *d = Get_Domain(1);	// Calculation domain
int zone_id = RP_Get_Real("wall_id"); 			// wall zone id
int shadow_zone_id = RP_Get_Real("shadow_wall_id");	// shadow wall zone id
face_t f;					// wall face
Thread *t1,*t2;					// Wall thread, interior wall cell thread
t1 = Lookup_Thread(d,zone_id); // Determine face thread for wall
t2 = Lookup_Thread(d,shadow_zone_id); // Determine face thread for shadow wall
real q; // heat flux over area
real y = RP_Get_Real("y_0"); // air gap width
real c = RP_Get_Real("c"); // air gap evolution constant

dx = RP_Get_Real("thickness");
temp2 = mean_wall_temp(t1);
temp1 = mean_wall_temp(t2);
q= (temp1-temp2)/ dx;
f_s = 1-RP_Get_Real("fl"); //get updated fraction of solid
y += c*f_s;	// update air gap width

// Calculate new wall thickness based on updated air gap width using Hamasaiid model
lambda = 2*k_m*k_c/(k_m+k_c);
alpha = y/(pow(2,1.0/2.0)*sigma);
n = 8*(1-erf(alpha))/(pow(PI,2)*epsilon*pow(r_sm,2));	
a = 1.0/4.0*r_sm*(2*exp(-pow(double(alpha),2.0))+(pow(double(2*PI),1.0/2.0)*y*(-1+erf(alpha))/sigma));
h = 2*lambda*a*n/pow(double(1-2*a/r_sm),3.0/2.0);
dx = RP_Get_Real("k_res")/h;

RP_Set_Real("thickness", dx);

//write output to output.txt
fp = fopen("output.txt","a");
fprintf(fp,"%g %g %g %g\n", RP_Get_Real("flow-time"), f_s, q, h);
fclose(fp);
}