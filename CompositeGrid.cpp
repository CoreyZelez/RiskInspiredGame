//#include "CompositeGrid.h"
//
//void CompositeGrid::draw(sf::RenderWindow & window)
//{
//}
//
//void CompositeGrid::addGrid(const Grid &grid)
//{
//	auto iter = grids.insert(grid).first;
//	addAdjacencies(*iter);
//	updateBorders(*iter, true);
//}
//
//// Removes a grid in grids that shares the same id as the paramter grid (i.e. are identically shaped).
//void CompositeGrid::removeGrid(const Grid &grid)
//{
//	auto iter = grids.begin();
//	while(iter != grids.end())
//	{
//		if(grid.sameId(*iter))
//		{
//			// assert(grids share identical positions. make a function for this.)
//			removeAdjacencies(*iter);
//			updateBorders(*iter, true);
//			grids.erase(iter);
//			break;
//		}
//
//		++iter;
//	}
//}
//
//// Adds the adjacencies to the grid.
//void CompositeGrid::addAdjacencies(const Grid &newGrid)
//{
//	for(const Grid &grid : grids)
//	{
//		if(&grid == &newGrid)
//		{
//			continue;
//		}
//		else if(newGrid.isAdjacent(grid))
//		{
//			gridAdjacencies[&grid].insert(&newGrid);
//			gridAdjacencies[&newGrid].insert(&grid);
//		}
//	}
//}
//
//// Removes the adjacencies with the grid.
//void CompositeGrid::removeAdjacencies(const Grid &newGrid)
//{
//	for(const Grid* grid : gridAdjacencies[&newGrid])
//	{
//		gridAdjacencies[grid].erase(&newGrid);
//	}
//
//	gridAdjacencies.erase(&newGrid);
//}
//
//void CompositeGrid::updateBorders(const Grid & grid, bool added)
//{
//}