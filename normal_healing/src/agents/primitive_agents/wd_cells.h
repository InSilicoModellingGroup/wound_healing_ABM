#ifndef wd_cells_H_
#define wd_cells_H_

#include "biodynamo.h"

namespace bdm { 
	
	class wd_cells : public Cell {
		BDM_AGENT_HEADER(wd_cells, Cell, 1);	
	
		public:
			wd_cells() {}
			explicit wd_cells(const Real3& position) : Base(position) {}
			virtual ~wd_cells() {}
		
		
		
			//Getters and Setters of private functions
			void SetCanDivide(bool a) { can_divide_ =  a; }   //can divide
  			bool GetCanDivide() const { return can_divide_; }

			void SetInsideWoundArea(bool b) { InsideWoundArea_ =  b; }  //InsideWoundArea
  			bool GetInsideWoundArea() const { return InsideWoundArea_; }

			void SetCellType(int c) { cell_type_ =  c; } // CellType
  			bool GetCellType() const { return cell_type_; }

			void SetColor(Real3 a) { 
				cell_color_[0] = a[0];
				cell_color_[1] = a[1];
				cell_color_[2] = a[2];
			}
				
			Real3 GetColor() { return cell_color_ ;}
			
			void SetInitialPositionInWA(bool h) { InitialPositionInWA_= h ;}
			bool GetInitialPositionInWA() const { return InitialPositionInWA_; }
			
			void SetItSecretes(bool niaou) {ItSecretes_ = niaou ;}
			bool GetItSecretes() const {return ItSecretes_;}
			
			void SetCellAge(int k) {cell_age_ = k;}
			int GetCellAge() { return cell_age_ ;}
			
			void SetCellOnBound(bool b) {cell_on_bound_ = b ;}
			bool GetCellOnBound() const {return cell_on_bound_;}
			
			void SetProbability(real_t h) {probability_ = h;}
			real_t GetProbability() {return probability_;}
		private:
			//True if cells are inside wound area
			bool InsideWoundArea_ = false;
			bool InitialPositionInWA_;
			bool ItSecretes_;
		
			//Can_divide
			bool can_divide_;
			
			bool cell_on_bound_ = false;
			
			real_t probability_;
			
			//Cell Type 
			int cell_type_;
			
			Real3 cell_color_ ; 
			
			size_t cell_age_;
	};

};
#endif //wd_cells_H_
