#ifndef FIBROBLAST_PROLIFERATION2_H_
#define FIBROBLAST_PROLIFERATION2_H_

#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"



namespace bdm {

	struct FibroblastProliferation2 : public Behavior {
		BDM_BEHAVIOR_HEADER(FibroblastProliferation2, Behavior, 12);
	
		public:
			FibroblastProliferation2() {
				//AlwaysCopyToNew();
				NeverRemoveFromExisting();
			}
			
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (FibroblastProliferation2* casted = dynamic_cast<FibroblastProliferation2*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("MacrophageProliferation::EventConstructor",
						"other was not of type MacrophageProliferation");
				}
			}
  			
  			void Run(Agent* agent) override {
  				if (auto* this_cell = dynamic_cast<fibroblast*>(agent)) {
					auto* rm = Simulation::GetActive()->GetResourceManager();
      				auto* dgrid = rm->GetDiffusionGrid(0); //platelet growth factor
      				real_t conc_PDGF = dgrid->GetValue(this_cell->GetPosition());
      					
      				auto* dgrid_TGFb = rm->GetDiffusionGrid(1);
      				real_t conc_TGFb = dgrid_TGFb->GetValue(this_cell->GetPosition());
      					
      				auto* rand = Simulation::GetActive()->GetRandom();

      					
      				if (conc_TGFb > GetProliThreshold()) {
      					if (rand->Uniform() > GetProliferationProbability()) {
      						this_cell->AddBehavior(new GrowthDivision(1.5,GetProliferationRate()));
      							
      						int nbs_fbrs = 0;
							auto check_for_fbrs = L2F([&](Agent* neighbor, real_t squared_distance) {
								if (squared_distance > squared_distance) {
									return;
								}
      							if (dynamic_cast<fibroblast*>(neighbor)) {
        							++nbs_fbrs;
        						}
							});
				 			auto* ctxt = Simulation::GetActive()->GetExecutionContext();
   							ctxt->ForEachNeighbor(check_for_fbrs, *this_cell, 2*2);

 							if (nbs_fbrs > GetlocalPopulationDensity() ) {
 								auto behaviors = this_cell->GetAllBehaviors();				
      								for (auto* b : behaviors) {
										if (dynamic_cast<GrowthDivision*>(b)) {
							    			this_cell->RemoveBehavior(b);   // removes *all* GD behaviors
										}
						    		}
      								this_cell -> RemoveBehavior(this);
      								//std::cout<<"nbs_fbrs"<<nbs_fbrs<<std::endl;	
    						}
      					}		
      				}
      					
      				if (conc_TGFb > GetlocalConcetrationDensity() || this_cell->GetPosition()[1] < abs(2*this_cell->GetPosition()[0] - 100) + 28)  {//2000=fibrosis
						auto behaviors = this_cell->GetAllBehaviors();
						// std::cout << "Num GD behaviors step= " << this_cell->GetAllBehaviors().size();
						for (auto* b : behaviors) {
							if (dynamic_cast<GrowthDivision*>(b)) {
								this_cell->RemoveBehavior(b);   // removes *all* GD behaviors
							}
						}
						//std::cout << " to " << this_cell->GetAllBehaviors().size() << "\n";
						// Now remove KeratinocyteProliferation
						this_cell->RemoveBehavior(this);   
					}	
    			}
  			}
  			
  			//getter functions
  			real_t GetProliferationProbability() {return proliferation_probability_;}
  			real_t GetProliThreshold() {return proliferation_threshold_;}
  			real_t GetProliferationRate() {return proliferation_rate_;}
  			real_t GetlocalPopulationDensity() {return local_population_density_;}
  			real_t GetlocalConcetrationDensity() {return local_conc_density_;}
  			
		private: 
			//cell position
			Real3 position_ ;
			
			real_t squared_radius_ = 0;
			
			real_t proliferation_probability_ = 0.6;
			
			real_t proliferation_threshold_ = 80;
			
			real_t proliferation_rate_ = 0.1;
			
			real_t local_population_density_ = 10;
			
			real_t local_conc_density_ = 1000;
	
	};


}//namespace bdm

#endif // FIBROBLAST_PROLIFERATION2_H_
