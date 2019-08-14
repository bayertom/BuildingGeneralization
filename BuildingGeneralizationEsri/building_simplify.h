#ifndef Building_Simplify_H
#define Building_Simplify_H

#include "resource.h"
#include "BuildingGeneralizationESRI_i.h"

#include "Edge.h"
#include "Point.h"
#include "MainDialog.h"
#include "Algorithms.h"

#define TOLERANCE 0.001   //tolerance definition
//extern std::ofstream file4;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// Cbuilding_simplify
class ATL_NO_VTABLE Cbuilding_simplify :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<Cbuilding_simplify, &CLSID_building_simplify>,
	public Ibuilding_simplify,
	public ICommand
{
public:
	Cbuilding_simplify()
	{
		//Load bitmap
		m_hBitmap = ::LoadBitmap(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDB_BITMAP1));
		//result=new Result();
		//edges=new EdgeList(result);
	}

	~Cbuilding_simplify()
	{
		//delete edges;
		//delete result;
		DeleteObject(m_hBitmap);
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_BUILDING_SIMPLIFY)


	BEGIN_COM_MAP(Cbuilding_simplify)
		COM_INTERFACE_ENTRY(Ibuilding_simplify)
		COM_INTERFACE_ENTRY(ICommand)
		COM_INTERFACE_ENTRY(ICommand)
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

private:
	bool split;
	double sigma;
	std::list <Point> points;

	HBITMAP  m_hBitmap;
	IApplicationPtr p_application;
	IMxDocumentPtr p_document;
	ILayerPtr p_layer; 
	IGeoFeatureLayerPtr  p_geolayer; 
	IFeatureLayer *p_featurelayer;
	IEnvelopePtr p_envelop;
	IActiveViewPtr p_view; 
	IFeatureRendererPtr p_renderer; 
	IFeatureClassPtr p_featureclass; 
	IFeatureBufferPtr p_featurebuffer; 
	ISpatialReferencePtr p_reference;
	IWorkspacePtr p_workspace; 
	IDatasetPtr p_dataset; 
	IGeoDatasetPtr p_geodataset;
	esriGeometryType p_geometry; 
	IFeatureSelectionPtr p_selection;
	ISelectionSetPtr p_selectionset;
	IFeatureCursorPtr p_featurecursor;
	IFeature *p_feature;
	IGeometryPtr p_shape;
	ICursorPtr p_cursor;
	IWorkspaceEditPtr p_workspaceeditor;
	IPointCollectionPtr p_collection; 
	IGeometryCollectionPtr p_geometrycollection;
	IGeometryPtr p_geometrycoll;
	IStatusBarPtr p_statusbar;
	IScreenDisplayPtr p_display;

	// ICommand Methods
public:
	STDMETHOD(get_Enabled)(VARIANT_BOOL * Enabled)
	{
		if (Enabled == NULL)
		{
			return E_POINTER;
		}
		*Enabled = VARIANT_TRUE;
		return S_OK;
	}

	STDMETHOD(get_Checked)(VARIANT_BOOL * Checked)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(get_Name)(BSTR * Name)
	{
		if (Name == NULL)
		{ 
			return E_POINTER;
		}
		else
		{
			*Name = ::SysAllocString(L"Odstranìní uzavøené oblasti rozdìlením (eliminace).");
			return S_OK;
		}
	}

	STDMETHOD(get_Caption)(BSTR * Caption)
	{
		//Caption of the tool
		if (Caption == NULL)
		{
			return E_POINTER;
		}
		else
		{
			*Caption = ::SysAllocString(L"Building simplify...");
			return S_OK;
		}
	}

	STDMETHOD(get_Tooltip)(BSTR * Tooltip)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(get_Message)(BSTR * Message)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(get_HelpFile)(BSTR * HelpFile)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(get_HelpContextID)(long * helpID)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(get_Bitmap)(OLE_HANDLE * Bitmap)
	{
		if (Bitmap == NULL)
		{
			return E_POINTER;
		}

		else
		{
			*Bitmap = (OLE_HANDLE) m_hBitmap;
			return S_OK;
		}
	}

	STDMETHOD(get_Category)(BSTR * categoryName)
	{
		if (categoryName == NULL)
		{
			return E_POINTER;
		}
		else
		{
			*categoryName = ::SysAllocString(L"Generalization");
			return S_OK;
		}
	}

	STDMETHOD(OnCreate)(LPDISPATCH hook)
	{
		//hook to application
		p_application = hook;

		//reference to document
		IDocumentPtr p_doc;
		HRESULT hr = p_application->get_Document(&p_doc);
		p_document=p_doc;

		//When fails
		if (p_document==NULL) 
		{
			MessageBox(0,L"Bad hook !", L"Error:", MB_OK);
			return E_FAIL;
		}

		else return S_OK;
	}

	STDMETHOD(OnClick)()
	{
		//Clear all
		CMainDialog dialog;
		INT_PTR ret=-1;
		
		//Create dialog
		ret=dialog.DoModal();
		
		//Close dialog
		HRESULT hr;
		switch(ret)
		{
		case -1: 
			MessageBoxW(NULL,L"Error during dialog creation...", L"Error",MB_OK);
			break;
			
		case IDOK:
			//Get values
			sigma=dialog.getSigma();
			split=dialog.getSplit();

			//Clear dynamic structure
			points.clear();
		
			hr=getFeatureClass();
			if (hr!=E_FAIL)
			{
				hr=setEdit();
				if (hr!=E_FAIL) 
				{
					hr=getSelection(0);
					if (hr!=E_FAIL) 
					{
						return S_OK;
					}
				}         
			}
			break;

		case IDYES:
			//Get values
			sigma=dialog.getSigma();
			split=dialog.getSplit();

			//Clear dynamic structure
			points.clear();

			hr=getFeatureClass();
			if (hr!=E_FAIL)
			{
				hr=setEdit();
				if (hr!=E_FAIL) 
				{
					hr=getSelection(1);
					if (hr!=E_FAIL) 
					{
						return S_OK;
					}
				}         
			}
			break;
		}

		return E_FAIL;
	}	


	HRESULT getFeatureClass()
	{
		//Get Selected Layer
		p_document->get_SelectedLayer(&p_layer);

		//Was layer got?
		if (p_layer==NULL) 
		{
			MessageBox(0,L"No layer has been selected !", L"Error", MB_OK);
			return E_FAIL;
		}

		//Active view
		p_document->get_ActiveView(&p_view);

		//Was active view got?
		if (p_view==NULL) 
		{
			return E_FAIL;
		}

		//Get screen display
		p_view->get_ScreenDisplay(&p_display);

		//Convert layer to geolayer
		p_geolayer = p_layer;

		//Get renderer
		p_geolayer->get_Renderer(&p_renderer);
		if (p_renderer==NULL)
		{
			return E_FAIL;
		}

		//Get feature class
		p_geolayer->get_FeatureClass(&p_featureclass);
		if (p_featureclass==NULL) 
		{
			return E_FAIL;
		}

		//Get geometry
		p_featureclass->get_ShapeType(&p_geometry);

		//Polygon was selected?
		if (p_geometry!=esriGeometryPolygon)
		{
			MessageBox(0,L"Selected layer does not contain any polygon !", L"Error !!!", MB_OK);
			return E_FAIL;
		}

		//Feature class to geodataset conversion
		p_geodataset=p_featureclass;

		//Get Spatial reference
		p_geodataset->get_SpatialReference(&p_reference);

		//Was spatial reference got?
		if (p_reference==NULL)
		{
			MessageBox(0,L"Spatial reference was not got !", L"Error!!!", MB_OK);
			return E_FAIL;
		}

		//Feature class to dataset conversion
		p_dataset=p_featureclass;

		//Create feature buffer
		HRESULT hr = p_featureclass->CreateFeatureBuffer(&p_featurebuffer);

		return S_OK;
	}

	

	HRESULT getSelection(int status)
	{
		//Get number of features
		long number_of_features;
		p_featureclass->FeatureCount(NULL, &number_of_features); 

		//Convert geolayer to feature layer
		p_featurelayer=p_geolayer;

		//Convert feature layer to selection
		p_selection =p_featurelayer;

		//Get selection
		p_selection->get_SelectionSet(&p_selectionset);

		//Get number of selected features
		long number_of_selected;

		//Get selection set
		p_selectionset->Search(0, VARIANT_FALSE, &p_cursor);
		if (p_cursor==NULL)
		{
			return E_FAIL;   
		}

		//Get number of selected items
		p_selectionset->get_Count(&number_of_selected);
		if (number_of_selected<1)
		{
			MessageBox(0,L"Select at least one polygon !", L"Error!", MB_OK);
			return E_FAIL;
		}

		//Convert cursor to feature cursor
		p_featurecursor=p_cursor;

		//Write to status bar
		p_application->get_StatusBar(&p_statusbar);
		p_statusbar->put_Message(0,SysAllocString(L"Buildings are simplifying, please wait..."));

		//Refresh view
		if (status==0) p_view->Refresh();

		//Repeat for selected polygons
		for (int i=0;i<number_of_selected;i++)
		{
			//Get next feature
			p_featurecursor->NextFeature(&p_feature);
			if (p_feature==NULL)
			{
				return E_FAIL;
			}

			//Get shape and geometry
			p_feature->get_Shape(&p_shape);
			if (p_shape==NULL)
			{
				return E_FAIL;
			}
		
			//Convert to geometry collection
			p_geometrycollection=p_shape;

			//Number of elements of the polygon
			long number_of_elements;
			p_geometrycollection->get_GeometryCount(&number_of_elements);

			//Info to status bar
			TCHAR buf[40]={0};
			wsprintf(buf,TEXT("Buildings are simplifying: %d %%"),int(i*100.0/number_of_selected));
			p_statusbar->put_Message(0,SysAllocString(buf));

			//Repeat for every element of polygon
			for (int j=0;j<number_of_elements;j++)
			{
				//Create topology graph
				//HRESULT hr = p_topologygraph->Build(p_envelop, VARIANT_FALSE);
				//if (FAILED(hr)) return hr; 

				//Get element geometry
				p_geometrycollection->get_Geometry(j,&p_geometrycoll);

				//Convert geometry to points collection
				p_collection=p_geometrycoll;

				//Number of vertices
				long number_of_vertices;
				p_collection->get_PointCount(&number_of_vertices);

				IPoint *point;
				int node;

				//Clear container
				points.clear();
				
				// For every vertex of the element
				for (int k=0;k<number_of_vertices-1;k++)
				{
					IPoint *point;
					p_collection->get_Point(k,&point);

					//Get coordinates
					double x,y;
					point->get_X(&x);
					point->get_Y(&y);

					Point p(x,y);
					p.setOrientation(0);
					points.push_back(p);
				}

				//Remove all points of the collection
				if (status==0) p_collection->RemovePoints(0,number_of_vertices);

				//Simplify polygon
				simplify(status);
			}
		}

		//Show message
		p_statusbar->put_Message(0,SysAllocString(L"Done..."));

		return S_OK;
	}

	HRESULT setEdit()
	{
		VARIANT_BOOL edit;

		//Get workspace
		HRESULT hr = p_dataset->get_Workspace(&p_workspace);
		if (FAILED(hr)) return hr;

		p_workspaceeditor=p_workspace;

		//Detection of edit
		p_workspaceeditor->IsBeingEdited(&edit);
		if (!edit)
		{
			MessageBox(0,L"Enable data edit !", L"Error!!!", MB_OK);
			return E_FAIL;
		}

		//Start edit
		p_workspaceeditor->StartEditOperation();

		//Was it succesfull?
		if (FAILED(p_featureclass->Insert(VARIANT_TRUE,&p_featurecursor))) 
		{ 
			p_workspaceeditor->AbortEditOperation(); 
			p_workspaceeditor->StopEditing(VARIANT_FALSE); 
			return E_FAIL; 
		} 


		//Create new feature buffer
		IFeatureBufferPtr p_featurebuffer;
		hr = p_featureclass->CreateFeatureBuffer(&p_featurebuffer);
		if (FAILED(hr)) return hr;

		return S_OK;
	}


	int main()
	{
		simplify(0);
	}

	void simplify(int status)
	{
		//Simplify building
		double angle;
		std::list<Edge> res;

		//file4 << "Simplify...\n";
		Algorithms::simplify(points, sigma, split, res, angle);

		if (status==0)
		{
			//Simplify
			Algorithms::drawBuilding(res, angle, p_collection);
		}
		
		//Proof simplifying without writing data
		Algorithms::drawBuildingTemp(res, angle, p_display);

		//Store results
		p_feature->Store();

		//Stop edit operation
		p_workspaceeditor->StopEditOperation();

		//Refresh view
		if (status==0) p_view->Refresh();
	}

};

OBJECT_ENTRY_AUTO(__uuidof(building_simplify), Cbuilding_simplify)

#endif