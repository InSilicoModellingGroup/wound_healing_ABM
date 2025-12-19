#ifndef EPITHELIAL_CELL_H_
#define EPITHELIAL_CELL_H_

#include "biodynamo.h"

namespace bdm {

	class epithelial_cell : public SphericalAgent {
		BDM_AGENT_HEADER(epithelial_cell, SphericalAgent, 5);
		
		public:
			epithelial_cell() {}
			explicit epithelial_cell(const Real3& position) :Base(position) {}
			
			
 			void SetColor(Real3 a) { 
				cell_color_[0] = a[0];
				cell_color_[1] = a[1];
				cell_color_[2] = a[2];
			}
    			Real3 GetColor() { return cell_color_ ;}
			
	  	private:
			Real3 cell_color_ ;
	  	
	  	
	};

}  // namespace bdm

#endif  // epithelial_cell_H_
