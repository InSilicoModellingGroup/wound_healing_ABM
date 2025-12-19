#ifndef FIBROBLAST_MIGRATION_H_
#define FIBROBLAST_MIGRATION_H_


#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"
#include "behaviors/random_walk_module.h"

namespace bdm {
	//check if the cell is inside the wound area
	struct FibroblastMigration : public Behavior {
		BDM_BEHAVIOR_HEADER(FibroblastMigration, Behavior, 13);
	
		public:
			FibroblastMigration() {
				AlwaysCopyToNew();
				NeverRemoveFromExisting();
			}
			
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (FibroblastMigration* casted = dynamic_cast<FibroblastMigration*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("PlateletSecretion::EventConstructor",
						"other was not of type MacrophageMigration");
				}
			}
  			uint64_t step = Simulation::GetActive()->GetScheduler()->GetSimulatedSteps();
  			
  			void Run(Agent* agent) override {
  				if (auto* fbr = dynamic_cast<fibroblast*>(agent)) {
  					if (fbr->GetCellOnBound() == true) {
  						fbr->RemoveBehavior(this);
  					}
  					else {
  						fbr->AddBehavior(new RandomWalk(0.5));
  					}
  	      			auto* rand = Simulation::GetActive()->GetRandom();
  	      			
  					if (fbr->GetProbability() > rand->Uniform()) {
  						fbr->AddBehavior(new Chemotaxis("TGF-b", GetMigrationRate()));
  					}	
  				}
  			}
  			real_t GetMigrationRate() {return migration_rate_;}
		private: 
			//cell position
			Real3 position_ ;
			
			real_t migration_rate_ = 0.002;//normale=0.002
			
			real_t migration_propability_ = 0.5;
	
	};


}//namespace bdm

#endif // MACROPHAGE_MIGRATION_H_
