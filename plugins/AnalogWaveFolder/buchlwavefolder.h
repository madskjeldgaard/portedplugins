// http://research.spa.aalto.fi/publications/papers/dafx17-wavefolder/

// History variables
History ph;
History Vk_ic_n1;

// Flags
History fl1;
History fl2;
History fl3;
History fl4;

Data cl_pts(4);
Data flags(4);

// Create flags vector
flags.poke(fl1,0,0);
flags.poke(fl2,1,0);
flags.poke(fl3,2,0);
flags.poke(fl4,3,0);

// Read fundamental freq and ampl
f0 = in1;
A = in2;
Fs = 44100;
Ts = 1/Fs;

thresh = 0.01;

// Compute clipping threshold L
Vs = 6;
R1 = in3;
R2 = 100e3;
R3 = in4;
L = Vs*(R1/R2);

// Dummy variable(s)
Vk_ic = 0;

// Helper constants
G = ((R2*R3)/(R1*R3 + R2*R3 + R1*R2));
one_sixth = 1.0/6.0;


// Phase increment
delta = f0/Fs;


// Synthesize input
xn = A*sin(2*pi*ph);

// Inverse Clipper
if (abs(xn)>L) {
	Vk_ic = xn;
}
else {
	Vk_ic = sign(xn)*L;
}

if (A>=L){

	// Compute clipping points
	clp1 = asin(L/A)/(2*pi);
	clp2 = 0.5 - clp1;
	clp3 = 0.5 + clp1;
	clp4 =   1 - clp1;
	cl_pts.poke(clp1,0,0);
	cl_pts.poke(clp2,1,0);
	cl_pts.poke(clp3,2,0);
	cl_pts.poke(clp4,3,0);

	// Reset counters and check for that really annoying first case.
	fl4 = flags.peek(3,0);

	if ((ph<delta) && (fl4==1)){
	
		flags.poke(0,0,0);
		flags.poke(0,1,0);
		flags.poke(0,2,0);
		flags.poke(0,3,0);
	
	}
	else if ((ph<delta) && (fl4==0)) {
	
		clp4 = cl_pts.peek(3,0);

		d = 1 - ((1-clp4+ph)/delta);

    	h0 = -(d*d*d)*one_sixth + 0.5*(d*d) - 0.5*d + one_sixth;
    	h1 = (d*d*d)*one_sixth;

    	pol = sign(A*sin(2*pi*clp4));       
    
		mu = abs(A*cos(2*pi*clp4)*(2*pi*f0*Ts));
		if (mu<thresh){
			mu = 0;
		}

    	Vk_ic = Vk_ic + pol*mu*h1;
    	Vk_ic_n1 = Vk_ic_n1 + pol*mu*h0;

		flags.poke(0,0,0);
		flags.poke(0,1,0);
		flags.poke(0,2,0);
		flags.poke(0,3,0);
	
	} 


	// CHECK FOR CLIPPING POINTS
	for(m=0; m<4; m=m+1){
	
		clp = cl_pts.peek(m,0);
		flg = flags.peek(m,0);

		if ((ph > clp) && (ph > (ph-delta)) && (flg==0)) {
			
        	d = 1 - (ph-clp)/delta;

        	h0 = -(d*d*d)*one_sixth + 0.5*(d*d) - 0.5*d + one_sixth;
        	h1 = (d*d*d)*one_sixth;

        	pol = sign(A*sin(2*pi*clp));
            
        	mu = abs(A*cos(2*pi*clp)*(2*pi*f0*Ts));
			if (mu<thresh){
					mu = 0;
			}

        	Vk_ic = Vk_ic + pol*mu*h1;
        	Vk_ic_n1 = Vk_ic_n1 + pol*mu*h0;
			
			flags.poke(1,m,0);
			
		}
	}
}

// Restore folding function
out1 = G*(Vk_ic_n1 - sign(Vk_ic_n1)*L);

// Update States
Vk_ic_n1 = Vk_ic;
fl1 = flags.peek(0,0);
fl2 = flags.peek(1,0);
fl3 = flags.peek(2,0);
fl4 = flags.peek(3,0);

// Increase counter
ph = mod(ph+delta,1);
