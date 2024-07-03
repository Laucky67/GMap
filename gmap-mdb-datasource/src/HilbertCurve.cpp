#include "HilbertCurve.h"

int HilbertCurve::mMapCodes[4][4]={
	{0,1,3,2},
	{0,2,3,1},
	{3,1,0,2},
	{3,2,0,1}};

int HilbertCurve::mInvertMapCodes[4][4]={
	{0,1,3,2},
	{0,3,1,2},
	{2,1,3,0},
	{2,3,1,0}};

int HilbertCurve::mGenCodes[4][4]={
	{1,0,0,2},
	{0,1,1,3},
	{3,2,2,0},
	{2,3,3,1}};

