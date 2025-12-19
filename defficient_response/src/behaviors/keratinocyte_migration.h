#ifndef KERATINOCYTE_MIGRATION_H_
#define KERATINOCYTE_MIGRATION_H_


#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"
#include "behaviors/random_walk_module.h"

namespace bdm {
	//check if the cell is inside the wound area
	struct KeratinocyteMigration : public Behavior {
		BDM_BEHAVIOR_HEADER(KeratinocyteMigration, Behavior, 14);
	
		public:
			KeratinocyteMigration() {
				AlwaysCopyToNew();
				NeverRemoveFromExisting();
			}
			
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (KeratinocyteMigration* casted = dynamic_cast<KeratinocyteMigration*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("PlateletSecretion::EventConstructor",
						"other was not of type MacrophageMigration");
				}
			}
  			uint64_t step = Simulation::GetActive()->GetScheduler()->GetSimulatedSteps();
  			
  			void Run(Agent* agent) override {
  				if (auto* this_cell = dynamic_cast<keratinocyte*>(agent)) {
  				
					bool found_fibroblast = false;
					int nbs_krt2fbrs_=0; //fbrs neighbors to a keratinocyte
					auto* rand = Simulation::GetActive()->GetRandom();
					
					//std::cout <<  this_cell->GetCellType() << std::end;
					auto check_for_fibroblast = L2F([&](Agent* neighbor, real_t squared_distance) {
						if (squared_distance > 2*2) {
							return;
						}
      						if (auto* nb = dynamic_cast<fibroblast*>(neighbor)) {
        						found_fibroblast = true;
        						//this_cell->SetColor(4);
        						this_cell ->AddBehavior(new RandomWalk(1));
        						++nbs_krt2fbrs_;
        						//std::cout<<"krt_found_fib"<<nbs_krt2fbrs_<<std::endl;
        						
        						if (nbs_krt2fbrs_ > 5 && rand->Uniform() > GetMigrationPropability()) {
      								this_cell -> SetActivated(true);
    							}
      						}
					});

    					auto* ctxt = Simulation::GetActive()->GetExecutionContext();
   					ctxt->ForEachNeighbor(check_for_fibroblast, *this_cell, 2*2);
					
					if (this_cell -> GetActivated()) {
						bool has_chemotaxis = false;
						for (auto* b : this_cell->GetAllBehaviors()) {
  							if (dynamic_cast<Chemotaxis*>(b)) {
    								has_chemotaxis = true;
    								break;
  							}
						}
						
						if (has_chemotaxis && this_cell->GetActivated()) {
  							//std::cout << "Chemotaxis exists on this cell.\n";
						} else {

  							this_cell->AddBehavior(new Chemotaxis("KGF", GetMigrationRate()));
						}
					}							
    						
  				}
  			}
  			real_t GetMigrationRate() {return migration_rate_;}
  			real_t GetMigrationPropability() {return migration_propability_;}
  			
		private: 
			//cell position
			Real3 position_ ;
			real_t migration_rate_ = 0.1;
			
			real_t migration_propability_ = 0.1;
	};


}//namespace bdm

#endif // MACROPHAGE_MIGRATION_H_
