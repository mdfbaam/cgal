// Copyright (c) 2009 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Stephane Tayeb
//
//******************************************************************************
// File Description : 
//******************************************************************************

#include "test_meshing_utilities.h"
#include <CGAL/Image_3.h>
#include <CGAL/Labeled_image_mesh_domain_3.h>
#include <CGAL/use.h>

template <typename K>
struct Image_tester : public Tester<K>
{
public:
  void image() const
  {
    typedef CGAL::Image_3 Image;
    typedef CGAL::Labeled_image_mesh_domain_3<Image, K> Mesh_domain;
    
    typedef typename CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
    typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;
    
    typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;
    typedef typename Mesh_criteria::Facet_criteria Facet_criteria;
    typedef typename Mesh_criteria::Cell_criteria Cell_criteria;
    
    CGAL_USE_TYPE(typename Mesh_domain::Surface_patch_index);
    
    //-------------------------------------------------------
    // Data generation
    //-------------------------------------------------------
    Image image;
    image.read("data/liver.inr.gz");
    Mesh_domain domain(image,1e-9);
    
    // Set mesh criteria
    Facet_criteria facet_criteria(25, 20*image.vx(), 5*image.vx());
    Cell_criteria cell_criteria(4, 25*image.vx());
    Mesh_criteria criteria(facet_criteria, cell_criteria);
    
    // Mesh generation
    C3t3 c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria,
                                        CGAL::parameters::no_exude(),
                                        CGAL::parameters::no_perturb());
    
    // Verify
    this->verify_c3t3_combinatorics(c3t3);
    this->verify(c3t3,domain,criteria);
  }

  template<typename C3t3>
  void verify_c3t3_combinatorics(const C3t3& c3t3) const
  {
    typedef typename C3t3::Triangulation        Tr;
    typedef typename Tr::Facet                  Facet;
    typedef typename Tr::Cell_handle            Cell_handle;
    typedef typename Tr::Finite_facets_iterator Finite_facets_iterator;
    typedef typename C3t3::Surface_patch_index  Surface_patch_index;

    for(Finite_facets_iterator fit =c3t3.triangulation().finite_facets_begin();
      fit != c3t3.triangulation().finite_facets_end();
      ++fit)
    {
      Facet f = *fit;
      Surface_patch_index index = c3t3.surface_patch_index(f);
      if(!c3t3.is_in_complex(f))
        assert(index == Surface_patch_index());
      else
      {
        Cell_handle c1 = f.first;
        Cell_handle c2 = f.first->neighbor(f.second);
        assert( (c1->subdomain_index() == index.first
          && c2->subdomain_index() == index.second)
          || (c2->subdomain_index() == index.first
          && c1->subdomain_index() == index.second));
      }
    }
  }
};



int main()
{
  Image_tester<K_e_i> test_epic;
  std::cerr << "Mesh generation from a 3D image:\n";
  test_epic.image();
  
  return EXIT_SUCCESS;  
}
