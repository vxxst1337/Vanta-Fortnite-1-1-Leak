#pragma once

struct fetures
{

	struct embot
	{
		float fovsize = 100;
		float smooth = 5;
		bool aimbot = false;
		bool prediction = false;
		bool fov = true;
	}; std::unique_ptr<embot> Aimbot = std::make_unique<embot>();

	struct vsul
	{
		bool box = true;
		bool boundingbox = false;
		bool bone_dump = false;
		bool Skeleton = false;
		bool radars = false;
		bool snapline = false;
		bool names = false;
		bool platform = false;

		struct rederrr
		{
			int radartype = 0;
			int radarsize = 300;
		}; std::unique_ptr<rederrr> Radar = std::make_unique<rederrr>();
	}; std::unique_ptr<vsul> Visuals = std::make_unique<vsul>();

	struct explt
	{
		bool ObjectNames = false;
		bool ProjectileTP = false;
		bool pfly = false;
	}; std::unique_ptr<explt> Exploits = std::make_unique<explt>();

	struct msc
	{

	}; std::unique_ptr<msc> Misc = std::make_unique<msc>();

}; std::unique_ptr<fetures> Features = std::make_unique<fetures>();


struct vers
{
	bool bIsMenuOpened = true;
}; std::unique_ptr<vers> Variables = std::make_unique<vers>();