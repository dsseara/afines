/*
 *  filament.cpp
 *  
 *
 *  Created by Simon Freedman on 12/22/2014
 *  Copyright 2014 University of Chicago. All rights reserved.
 *
 */

//=====================================
//include guard
#ifndef __FILAMENT_H_INCLUDED__
#define __FILAMENT_H_INCLUDED__

//=====================================
// forward declared dependencies

//=====================================
//included dependences
#include "string"
#include "vector"
#include "actin.h"
#include "Link.h"
#include "globals.h"
//=====================================
//actin filament class
class filament
{
    public:

        filament(double x0, double y0, double phi0, int nrod, double fovx, double fovy, int nx, int ny, 
                double vis, double deltat, double temp, bool isStraight,
                double rodLength, double linkLength, double stretching, double bending, double fracture); 

        filament(std::vector<actin *> rodvec, double linkLength, double stretching_stiffness, double bending_stiffness, 
                double deltat, double temp, double fracture, double gamma);
       
        filament();
        
        ~filament();
    
        void set_shear(double g);

        void update(double t);
        
        void update_bending();
        
        std::vector<filament *> update_stretching();

        void update_shear(); 
        
        actin * get_rod(int i);
        
        Link * get_link(int i);

        int get_nlinks();

        std::vector<std::vector<std::vector<int> > > get_quadrants();
        
        std::string write_rods();
        
        std::string write_links();
        
        std::string to_string();
        
        std::vector<actin *> get_rods(unsigned int first, unsigned int last);
        
        std::vector<filament *> fracture(int node);
        
        void update_forces(int index, double f1, double f2, double f3);
        
        bool operator==(const filament& that);
        
        void add_rod(actin * a);

    protected:
        
        double fov[2], gamma, temperature, dt, fracture_force, viscosity;
        
        int nq[2];

        std::vector<actin *> rods;
        
        std::vector<Link *> lks;

        std::string tostring;
};

// Filament class that is closer to the Nedelec and Foethke model than the above one

class NFfilament : public filament
{
    public:
        
        std::vector<double> get_P_matrix();

        std::vector<double> get_A_matrix();

        std::vector<double> get_B_matrix();

        std::vector<double> get_G_matrix();

        double* get_mobility();
        
        std::vector<double *> get_mobility_matrix();

        double get_tau();
        
        std::vector<NFfilament *> update_stretching();

        void update(double t);
        
        map<array<int,2>, double> get_J_matrix();

};

// use mid links to update bending

class DLfilament : public filament
{
    public:
        
        DLfilament(double x0, double y0, double phi0, int nrod, double fovx, double fovy, int nx, int ny, 
                double vis, double deltat, double temp, bool isStraight,
                double rodLength, double linkLength, double stretching, double bending, double fracture, double bending_fracture); 

        DLfilament(std::vector<actin *> rodvec, double linkLength, double stretching_stiffness, double bending_stiffness, 
                double deltat, double temp, double fracture, double bending_fracture, double gamma);


        std::vector<DLfilament *> update_bending();
        
        std::vector<DLfilament *> update_stretching();
        
        std::vector<DLfilament *> fracture(int node);
            
    private:

        vector<MidLink *> midlks;
        double bending_fracture_force;
};

#endif

