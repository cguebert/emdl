#include "SOPClasses.h"

#include <boost/container/flat_map.hpp>

#include <algorithm>
#include <vector>

namespace
{

	struct SOPClassStruct
	{
		emdl::SOP_Class sopClass;
		std::string uid, description;
	};
	using SOPClassStructs = std::vector<SOPClassStruct>;
	const SOPClassStructs& getSOPClassStructs()
	{
		using SOP = emdl::SOP_Class;
		static SOPClassStructs sops = {
			{ SOP::Unknown,                                                            "Not supported",                    "Not supported" },
			{ SOP::Verification,                                                       "1.2.840.10008.1.1",                "Verification SOP Class" },
			{ SOP::StorageCommitmentPushModel,                                         "1.2.840.10008.1.20.1",             "Storage Commitment Push Model SOP Class" },
			{ SOP::MediaStorageDirectoryStorage,                                       "1.2.840.10008.1.3.10",             "Media Storage Directory Storage" },
			{ SOP::ProceduralEventLogging,                                             "1.2.840.10008.1.40",               "Procedural Event Logging SOP Class" },
			{ SOP::SubstanceAdministrationLogging,                                     "1.2.840.10008.1.42",               "Substance Administration Logging SOP Class" },
			{ SOP::ModalityPerformedProcedureStep,                                     "1.2.840.10008.3.1.2.3.3",          "Modality Performed Procedure Step SOP Class" },
			{ SOP::ModalityPerformedProcedureStepRetrieve,                             "1.2.840.10008.3.1.2.3.4",          "Modality Performed Procedure Step Retrieve SOP Class" },
			{ SOP::ModalityPerformedProcedureStepNotification,                         "1.2.840.10008.3.1.2.3.5",          "Modality Performed Procedure Step Notification SOP Class" },
			{ SOP::BasicFilmSession,                                                   "1.2.840.10008.5.1.1.1",            "Basic Film Session SOP Class" },
			{ SOP::PrintJob,                                                           "1.2.840.10008.5.1.1.14",           "Print Job SOP Class" },
			{ SOP::BasicAnnotationBox,                                                 "1.2.840.10008.5.1.1.15",           "Basic Annotation Box SOP Class" },
			{ SOP::Printer,                                                            "1.2.840.10008.5.1.1.16",           "Printer SOP Class" },
			{ SOP::PrinterConfigurationRetrieval,                                      "1.2.840.10008.5.1.1.16.376",       "Printer Configuration Retrieval SOP Class" },
			{ SOP::BasicColorPrintManagementMeta,                                      "1.2.840.10008.5.1.1.18",           "Basic Color Print Management Meta SOP Class" },
			{ SOP::BasicFilmBox,                                                       "1.2.840.10008.5.1.1.2",            "Basic Film Box SOP Class" },
			{ SOP::VOILUTBox,                                                          "1.2.840.10008.5.1.1.22",           "VOI LUT Box SOP Class" },
			{ SOP::PresentationLUT,                                                    "1.2.840.10008.5.1.1.23",           "Presentation LUT SOP Class" },
			{ SOP::MediaCreationManagementUID,                                         "1.2.840.10008.5.1.1.33",           "Media Creation Management SOP Class UID" },
			{ SOP::BasicGrayscaleImageBox,                                             "1.2.840.10008.5.1.1.4",            "Basic Grayscale Image Box SOP Class" },
			{ SOP::BasicColorImageBox,                                                 "1.2.840.10008.5.1.1.4.1",          "Basic Color Image Box SOP Class" },
			{ SOP::DisplaySystem,                                                      "1.2.840.10008.5.1.1.40",           "Display System SOP Class" },
			{ SOP::BasicGrayscalePrintManagementMeta,                                  "1.2.840.10008.5.1.1.9",            "Basic Grayscale Print Management Meta SOP Class" },
			{ SOP::ComputedRadiographyImageStorage,                                    "1.2.840.10008.5.1.4.1.1.1",        "Computed Radiography Image Storage" },
			{ SOP::DigitalXRayImageStorageForPresentation,                             "1.2.840.10008.5.1.4.1.1.1.1",      "Digital X-Ray Image Storage - For Presentation" },
			{ SOP::DigitalXRayImageStorageForProcessing,                               "1.2.840.10008.5.1.4.1.1.1.1.1",    "Digital X-Ray Image Storage - For Processing" },
			{ SOP::DigitalMammographyXRayImageStorageForPresentation,                  "1.2.840.10008.5.1.4.1.1.1.2",      "Digital Mammography X-Ray Image Storage - For Presentation" },
			{ SOP::DigitalMammographyXRayImageStorageForProcessing,                    "1.2.840.10008.5.1.4.1.1.1.2.1",    "Digital Mammography X-Ray Image Storage - For Processing" },
			{ SOP::DigitalIntraOralXRayImageStorageForPresentation,                    "1.2.840.10008.5.1.4.1.1.1.3",      "Digital Intra-Oral X-Ray Image Storage - For Presentation" },
			{ SOP::DigitalIntraOralXRayImageStorageForProcessing,                      "1.2.840.10008.5.1.4.1.1.1.3.1",    "Digital Intra-Oral X-Ray Image Storage - For Processing" },
			{ SOP::EncapsulatedPDFStorage,                                             "1.2.840.10008.5.1.4.1.1.104.1",    "Encapsulated PDF Storage" },
			{ SOP::EncapsulatedCDAStorage,                                             "1.2.840.10008.5.1.4.1.1.104.2",    "Encapsulated CDA Storage" },
			{ SOP::GrayscaleSoftcopyPresentationStateStorage,                          "1.2.840.10008.5.1.4.1.1.11.1",     "Grayscale Softcopy Presentation State Storage SOP Class" },
			{ SOP::ColorSoftcopyPresentationStateStorage,                              "1.2.840.10008.5.1.4.1.1.11.2",     "Color Softcopy Presentation State Storage SOP Class" },
			{ SOP::PseudoColorSoftcopyPresentationStateStorage,                        "1.2.840.10008.5.1.4.1.1.11.3",     "Pseudo-Color Softcopy Presentation State Storage SOP Class" },
			{ SOP::BlendingSoftcopyPresentationStateStorage,                           "1.2.840.10008.5.1.4.1.1.11.4",     "Blending Softcopy Presentation State Storage SOP Class" },
			{ SOP::XAXRFGrayscaleSoftcopyPresentationStateStorage,                     "1.2.840.10008.5.1.4.1.1.11.5",     "XA/XRF Grayscale Softcopy Presentation State Storage" },
			{ SOP::GrayscalePlanarMPRVolumetricPresentationStateStorage,               "1.2.840.10008.5.1.4.1.1.11.6",     "Grayscale Planar MPR Volumetric Presentation State Storage" },
			{ SOP::CompositingPlanarMPRVolumetricPresentationStateStorage,             "1.2.840.10008.5.1.4.1.1.11.7",     "Compositing Planar MPR Volumetric Presentation State Storage" },
			{ SOP::XRayAngiographicImageStorage,                                       "1.2.840.10008.5.1.4.1.1.12.1",     "X-Ray Angiographic Image Storage" },
			{ SOP::EnhancedXAImageStorage,                                             "1.2.840.10008.5.1.4.1.1.12.1.1",   "Enhanced XA Image Storage" },
			{ SOP::XRayRadiofluoroscopicImageStorage,                                  "1.2.840.10008.5.1.4.1.1.12.2",     "X-Ray Radiofluoroscopic Image Storage" },
			{ SOP::EnhancedXRFImageStorage,                                            "1.2.840.10008.5.1.4.1.1.12.2.1",   "Enhanced XRF Image Storage" },
			{ SOP::PositronEmissionTomographyImageStorage,                             "1.2.840.10008.5.1.4.1.1.128",      "Positron Emission Tomography Image Storage" },
			{ SOP::LegacyConvertedEnhancedPETImageStorage,                             "1.2.840.10008.5.1.4.1.1.128.1",    "Legacy Converted Enhanced PET Image Storage" },
			{ SOP::XRay3DAngiographicImageStorage,                                     "1.2.840.10008.5.1.4.1.1.13.1.1",   "X-Ray 3D Angiographic Image Storage" },
			{ SOP::XRay3DCraniofacialImageStorage,                                     "1.2.840.10008.5.1.4.1.1.13.1.2",   "X-Ray 3D Craniofacial Image Storage" },
			{ SOP::BreastTomosynthesisImageStorage,                                    "1.2.840.10008.5.1.4.1.1.13.1.3",   "Breast Tomosynthesis Image Storage" },
			{ SOP::BreastProjectionXRayImageStorageForPresentation,                    "1.2.840.10008.5.1.4.1.1.13.1.4",   "Breast Projection X-Ray Image Storage - For Presentation" },
			{ SOP::BreastProjectionXRayImageStorageForProcessing,                      "1.2.840.10008.5.1.4.1.1.13.1.5",   "Breast Projection X-Ray Image Storage - For Processing" },
			{ SOP::EnhancedPETImageStorage,                                            "1.2.840.10008.5.1.4.1.1.130",      "Enhanced PET Image Storage" },
			{ SOP::BasicStructuredDisplayStorage,                                      "1.2.840.10008.5.1.4.1.1.131",      "Basic Structured Display Storage" },
			{ SOP::IntravascularOpticalCoherenceTomographyImageStorageForPresentation, "1.2.840.10008.5.1.4.1.1.14.1",     "Intravascular Optical Coherence Tomography Image Storage - For Presentation" },
			{ SOP::IntravascularOpticalCoherenceTomographyImageStorageForProcessing,   "1.2.840.10008.5.1.4.1.1.14.2",     "Intravascular Optical Coherence Tomography Image Storage - For Processing" },
			{ SOP::CTImageStorage,                                                     "1.2.840.10008.5.1.4.1.1.2",        "CT Image Storage" },
			{ SOP::EnhancedCTImageStorage,                                             "1.2.840.10008.5.1.4.1.1.2.1",      "Enhanced CT Image Storage" },
			{ SOP::LegacyConvertedEnhancedCTImageStorage,                              "1.2.840.10008.5.1.4.1.1.2.2",      "Legacy Converted Enhanced CT Image Storage" },
			{ SOP::NuclearMedicineImageStorage,                                        "1.2.840.10008.5.1.4.1.1.20",       "Nuclear Medicine Image Storage" },
			{ SOP::CTDefinedProcedureProtocolStorage,                                  "1.2.840.10008.5.1.4.1.1.200.1",    "CT Defined Procedure Protocol Storage" },
			{ SOP::CTPerformedProcedureProtocolStorage,                                "1.2.840.10008.5.1.4.1.1.200.2",    "CT Performed Procedure Protocol Storage" },
			{ SOP::UltrasoundMultiframeImageStorage,                                   "1.2.840.10008.5.1.4.1.1.3.1",      "Ultrasound Multi-frame Image Storage" },
			{ SOP::ParametricMapStorage,                                               "1.2.840.10008.5.1.4.1.1.30",       "Parametric Map Storage" },
			{ SOP::MRImageStorage,                                                     "1.2.840.10008.5.1.4.1.1.4",        "MR Image Storage" },
			{ SOP::EnhancedMRImageStorage,                                             "1.2.840.10008.5.1.4.1.1.4.1",      "Enhanced MR Image Storage" },
			{ SOP::MRSpectroscopyStorage,                                              "1.2.840.10008.5.1.4.1.1.4.2",      "MR Spectroscopy Storage" },
			{ SOP::EnhancedMRColorImageStorage,                                        "1.2.840.10008.5.1.4.1.1.4.3",      "Enhanced MR Color Image Storage" },
			{ SOP::LegacyConvertedEnhancedMRImageStorage,                              "1.2.840.10008.5.1.4.1.1.4.4",      "Legacy Converted Enhanced MR Image Storage" },
			{ SOP::RTImageStorage,                                                     "1.2.840.10008.5.1.4.1.1.481.1",    "RT Image Storage" },
			{ SOP::RTDoseStorage,                                                      "1.2.840.10008.5.1.4.1.1.481.2",    "RT Dose Storage" },
			{ SOP::RTStructureSetStorage,                                              "1.2.840.10008.5.1.4.1.1.481.3",    "RT Structure Set Storage" },
			{ SOP::RTBeamsTreatmentRecordStorage,                                      "1.2.840.10008.5.1.4.1.1.481.4",    "RT Beams Treatment Record Storage" },
			{ SOP::RTPlanStorage,                                                      "1.2.840.10008.5.1.4.1.1.481.5",    "RT Plan Storage" },
			{ SOP::RTBrachyTreatmentRecordStorage,                                     "1.2.840.10008.5.1.4.1.1.481.6",    "RT Brachy Treatment Record Storage" },
			{ SOP::RTTreatmentSummaryRecordStorage,                                    "1.2.840.10008.5.1.4.1.1.481.7",    "RT Treatment Summary Record Storage" },
			{ SOP::RTIonPlanStorage,                                                   "1.2.840.10008.5.1.4.1.1.481.8",    "RT Ion Plan Storage" },
			{ SOP::RTIonBeamsTreatmentRecordStorage,                                   "1.2.840.10008.5.1.4.1.1.481.9",    "RT Ion Beams Treatment Record Storage" },
			{ SOP::DICOSCTImageStorage,                                                "1.2.840.10008.5.1.4.1.1.501.1",    "DICOS CT Image Storage" },
			{ SOP::DICOSDigitalXRayImageStorageForPresentation,                        "1.2.840.10008.5.1.4.1.1.501.2.1",  "DICOS Digital X-Ray Image Storage - For Presentation" },
			{ SOP::DICOSDigitalXRayImageStorageForProcessing,                          "1.2.840.10008.5.1.4.1.1.501.2.2",  "DICOS Digital X-Ray Image Storage - For Processing" },
			{ SOP::DICOSThreatDetectionReportStorage,                                  "1.2.840.10008.5.1.4.1.1.501.3",    "DICOS Threat Detection Report Storage" },
			{ SOP::DICOS2DAITStorage,                                                  "1.2.840.10008.5.1.4.1.1.501.4",    "DICOS 2D AIT Storage" },
			{ SOP::DICOS3DAITStorage,                                                  "1.2.840.10008.5.1.4.1.1.501.5",    "DICOS 3D AIT Storage" },
			{ SOP::DICOSQuadrupoleResonanceQRStorage,                                  "1.2.840.10008.5.1.4.1.1.501.6",    "DICOS Quadrupole Resonance (QR) Storage" },
			{ SOP::UltrasoundImageStorage,                                             "1.2.840.10008.5.1.4.1.1.6.1",      "Ultrasound Image Storage" },
			{ SOP::EnhancedUSVolumeStorage,                                            "1.2.840.10008.5.1.4.1.1.6.2",      "Enhanced US Volume Storage" },
			{ SOP::EddyCurrentImageStorage,                                            "1.2.840.10008.5.1.4.1.1.601.1",    "Eddy Current Image Storage" },
			{ SOP::EddyCurrentMultiframeImageStorage,                                  "1.2.840.10008.5.1.4.1.1.601.2",    "Eddy Current Multi-frame Image Storage" },
			{ SOP::RawDataStorage,                                                     "1.2.840.10008.5.1.4.1.1.66",       "Raw Data Storage" },
			{ SOP::SpatialRegistrationStorage,                                         "1.2.840.10008.5.1.4.1.1.66.1",     "Spatial Registration Storage" },
			{ SOP::SpatialFiducialsStorage,                                            "1.2.840.10008.5.1.4.1.1.66.2",     "Spatial Fiducials Storage" },
			{ SOP::DeformableSpatialRegistrationStorage,                               "1.2.840.10008.5.1.4.1.1.66.3",     "Deformable Spatial Registration Storage" },
			{ SOP::SegmentationStorage,                                                "1.2.840.10008.5.1.4.1.1.66.4",     "Segmentation Storage" },
			{ SOP::SurfaceSegmentationStorage,                                         "1.2.840.10008.5.1.4.1.1.66.5",     "Surface Segmentation Storage" },
			{ SOP::TractographyResultsStorage,                                         "1.2.840.10008.5.1.4.1.1.66.6",     "Tractography Results Storage" },
			{ SOP::RealWorldValueMappingStorage,                                       "1.2.840.10008.5.1.4.1.1.67",       "Real World Value Mapping Storage" },
			{ SOP::SurfaceScanMeshStorage,                                             "1.2.840.10008.5.1.4.1.1.68.1",     "Surface Scan Mesh Storage" },
			{ SOP::SurfaceScanPointCloudStorage,                                       "1.2.840.10008.5.1.4.1.1.68.2",     "Surface Scan Point Cloud Storage" },
			{ SOP::SecondaryCaptureImageStorage,                                       "1.2.840.10008.5.1.4.1.1.7",        "Secondary Capture Image Storage" },
			{ SOP::MultiframeSingleBitSecondaryCaptureImageStorage,                    "1.2.840.10008.5.1.4.1.1.7.1",      "Multi-frame Single Bit Secondary Capture Image Storage" },
			{ SOP::MultiframeGrayscaleByteSecondaryCaptureImageStorage,                "1.2.840.10008.5.1.4.1.1.7.2",      "Multi-frame Grayscale Byte Secondary Capture Image Storage" },
			{ SOP::MultiframeGrayscaleWordSecondaryCaptureImageStorage,                "1.2.840.10008.5.1.4.1.1.7.3",      "Multi-frame Grayscale Word Secondary Capture Image Storage" },
			{ SOP::MultiframeTrueColorSecondaryCaptureImageStorage,                    "1.2.840.10008.5.1.4.1.1.7.4",      "Multi-frame True Color Secondary Capture Image Storage" },
			{ SOP::VLEndoscopicImageStorage,                                           "1.2.840.10008.5.1.4.1.1.77.1.1",   "VL Endoscopic Image Storage" },
			{ SOP::VideoEndoscopicImageStorage,                                        "1.2.840.10008.5.1.4.1.1.77.1.1.1", "Video Endoscopic Image Storage" },
			{ SOP::VLMicroscopicImageStorage,                                          "1.2.840.10008.5.1.4.1.1.77.1.2",   "VL Microscopic Image Storage" },
			{ SOP::VideoMicroscopicImageStorage,                                       "1.2.840.10008.5.1.4.1.1.77.1.2.1", "Video Microscopic Image Storage" },
			{ SOP::VLSlideCoordinatesMicroscopicImageStorage,                          "1.2.840.10008.5.1.4.1.1.77.1.3",   "VL Slide-Coordinates Microscopic Image Storage" },
			{ SOP::VLPhotographicImageStorage,                                         "1.2.840.10008.5.1.4.1.1.77.1.4",   "VL Photographic Image Storage" },
			{ SOP::VideoPhotographicImageStorage,                                      "1.2.840.10008.5.1.4.1.1.77.1.4.1", "Video Photographic Image Storage" },
			{ SOP::OphthalmicPhotography8BitImageStorage,                              "1.2.840.10008.5.1.4.1.1.77.1.5.1", "Ophthalmic Photography 8 Bit Image Storage" },
			{ SOP::OphthalmicPhotography16BitImageStorage,                             "1.2.840.10008.5.1.4.1.1.77.1.5.2", "Ophthalmic Photography 16 Bit Image Storage" },
			{ SOP::StereometricRelationshipStorage,                                    "1.2.840.10008.5.1.4.1.1.77.1.5.3", "Stereometric Relationship Storage" },
			{ SOP::OphthalmicTomographyImageStorage,                                   "1.2.840.10008.5.1.4.1.1.77.1.5.4", "Ophthalmic Tomography Image Storage" },
			{ SOP::WideFieldOphthalmicPhotographyStereographicProjectionImageStorage,  "1.2.840.10008.5.1.4.1.1.77.1.5.5", "Wide Field Ophthalmic Photography Stereographic Projection Image Storage" },
			{ SOP::WideFieldOphthalmicPhotography3DCoordinatesImageStorage,            "1.2.840.10008.5.1.4.1.1.77.1.5.6", "Wide Field Ophthalmic Photography 3D Coordinates Image Storage" },
			{ SOP::VLWholeSlideMicroscopyImageStorage,                                 "1.2.840.10008.5.1.4.1.1.77.1.6",   "VL Whole Slide Microscopy Image Storage" },
			{ SOP::LensometryMeasurementsStorage,                                      "1.2.840.10008.5.1.4.1.1.78.1",     "Lensometry Measurements Storage" },
			{ SOP::AutorefractionMeasurementsStorage,                                  "1.2.840.10008.5.1.4.1.1.78.2",     "Autorefraction Measurements Storage" },
			{ SOP::KeratometryMeasurementsStorage,                                     "1.2.840.10008.5.1.4.1.1.78.3",     "Keratometry Measurements Storage" },
			{ SOP::SubjectiveRefractionMeasurementsStorage,                            "1.2.840.10008.5.1.4.1.1.78.4",     "Subjective Refraction Measurements Storage" },
			{ SOP::VisualAcuityMeasurementsStorage,                                    "1.2.840.10008.5.1.4.1.1.78.5",     "Visual Acuity Measurements Storage" },
			{ SOP::SpectaclePrescriptionReportStorage,                                 "1.2.840.10008.5.1.4.1.1.78.6",     "Spectacle Prescription Report Storage" },
			{ SOP::OphthalmicAxialMeasurementsStorage,                                 "1.2.840.10008.5.1.4.1.1.78.7",     "Ophthalmic Axial Measurements Storage" },
			{ SOP::IntraocularLensCalculationsStorage,                                 "1.2.840.10008.5.1.4.1.1.78.8",     "Intraocular Lens Calculations Storage" },
			{ SOP::MacularGridThicknessandVolumeReportStorage,                         "1.2.840.10008.5.1.4.1.1.79.1",     "Macular Grid Thickness and Volume Report Storage" },
			{ SOP::OphthalmicVisualFieldStaticPerimetryMeasurementsStorage,            "1.2.840.10008.5.1.4.1.1.80.1",     "Ophthalmic Visual Field Static Perimetry Measurements Storage" },
			{ SOP::OphthalmicThicknessMapStorage,                                      "1.2.840.10008.5.1.4.1.1.81.1",     "Ophthalmic Thickness Map Storage" },
			{ SOP::CornealTopographyMapStorage,                                        "1.2.840.10008.5.1.4.1.1.82.1",     "Corneal Topography Map Storage" },
			{ SOP::BasicTextSRStorage,                                                 "1.2.840.10008.5.1.4.1.1.88.11",    "Basic Text SR Storage" },
			{ SOP::EnhancedSRStorage,                                                  "1.2.840.10008.5.1.4.1.1.88.22",    "Enhanced SR Storage" },
			{ SOP::ComprehensiveSRStorage,                                             "1.2.840.10008.5.1.4.1.1.88.33",    "Comprehensive SR Storage" },
			{ SOP::Comprehensive3DSRStorage,                                           "1.2.840.10008.5.1.4.1.1.88.34",    "Comprehensive 3D SR Storage" },
			{ SOP::ExtensibleSRStorage,                                                "1.2.840.10008.5.1.4.1.1.88.35",    "Extensible SR Storage" },
			{ SOP::ProcedureLogStorage,                                                "1.2.840.10008.5.1.4.1.1.88.40",    "Procedure Log Storage" },
			{ SOP::MammographyCADSRStorage,                                            "1.2.840.10008.5.1.4.1.1.88.50",    "Mammography CAD SR Storage" },
			{ SOP::KeyObjectSelectionDocumentStorage,                                  "1.2.840.10008.5.1.4.1.1.88.59",    "Key Object Selection Document Storage" },
			{ SOP::ChestCADSRStorage,                                                  "1.2.840.10008.5.1.4.1.1.88.65",    "Chest CAD SR Storage" },
			{ SOP::XRayRadiationDoseSRStorage,                                         "1.2.840.10008.5.1.4.1.1.88.67",    "X-Ray Radiation Dose SR Storage" },
			{ SOP::RadiopharmaceuticalRadiationDoseSRStorage,                          "1.2.840.10008.5.1.4.1.1.88.68",    "Radiopharmaceutical Radiation Dose SR Storage" },
			{ SOP::ColonCADSRStorage,                                                  "1.2.840.10008.5.1.4.1.1.88.69",    "Colon CAD SR Storage" },
			{ SOP::ImplantationPlanSRStorage,                                          "1.2.840.10008.5.1.4.1.1.88.70",    "Implantation Plan SR Storage" },
			{ SOP::AcquisitionContextSRStorage,                                        "1.2.840.10008.5.1.4.1.1.88.71",    "Acquisition Context SR Storage" },
			{ SOP::SimplifiedAdultEchoSRStorage,                                       "1.2.840.10008.5.1.4.1.1.88.72",    "Simplified Adult Echo SR Storage" },
			{ SOP::TwelveLeadECGWaveformStorage,                                       "1.2.840.10008.5.1.4.1.1.9.1.1",    "12-lead ECG Waveform Storage" },
			{ SOP::GeneralECGWaveformStorage,                                          "1.2.840.10008.5.1.4.1.1.9.1.2",    "General ECG Waveform Storage" },
			{ SOP::AmbulatoryECGWaveformStorage,                                       "1.2.840.10008.5.1.4.1.1.9.1.3",    "Ambulatory ECG Waveform Storage" },
			{ SOP::HemodynamicWaveformStorage,                                         "1.2.840.10008.5.1.4.1.1.9.2.1",    "Hemodynamic Waveform Storage" },
			{ SOP::CardiacElectrophysiologyWaveformStorage,                            "1.2.840.10008.5.1.4.1.1.9.3.1",    "Cardiac Electrophysiology Waveform Storage" },
			{ SOP::BasicVoiceAudioWaveformStorage,                                     "1.2.840.10008.5.1.4.1.1.9.4.1",    "Basic Voice Audio Waveform Storage" },
			{ SOP::GeneralAudioWaveformStorage,                                        "1.2.840.10008.5.1.4.1.1.9.4.2",    "General Audio Waveform Storage" },
			{ SOP::ArterialPulseWaveformStorage,                                       "1.2.840.10008.5.1.4.1.1.9.5.1",    "Arterial Pulse Waveform Storage" },
			{ SOP::RespiratoryWaveformStorage,                                         "1.2.840.10008.5.1.4.1.1.9.6.1",    "Respiratory Waveform Storage" },
			{ SOP::ContentAssessmentResultsStorage,                                    "1.2.840.10008.5.1.4.1.1.90.1",     "Content Assessment Results Storage" },
			{ SOP::PatientRootQR_Find,                                                 "1.2.840.10008.5.1.4.1.2.1.1",      "Patient Root Query/Retrieve Information Model - FIND" },
			{ SOP::PatientRootQR_Move,                                                 "1.2.840.10008.5.1.4.1.2.1.2",      "Patient Root Query/Retrieve Information Model - MOVE" },
			{ SOP::PatientRootQR_Get,                                                  "1.2.840.10008.5.1.4.1.2.1.3",      "Patient Root Query/Retrieve Information Model - GET" },
			{ SOP::StudyRootQR_Find,                                                   "1.2.840.10008.5.1.4.1.2.2.1",      "Study Root Query/Retrieve Information Model - FIND" },
			{ SOP::StudyRootQR_Move,                                                   "1.2.840.10008.5.1.4.1.2.2.2",      "Study Root Query/Retrieve Information Model - MOVE" },
			{ SOP::StudyRootQR_Get,                                                    "1.2.840.10008.5.1.4.1.2.2.3",      "Study Root Query/Retrieve Information Model - GET" },
			{ SOP::CompositeInstanceRootRetrieve_Move,                                 "1.2.840.10008.5.1.4.1.2.4.2",      "Composite Instance Root Retrieve - MOVE" },
			{ SOP::CompositeInstanceRootRetrieve_Get,                                  "1.2.840.10008.5.1.4.1.2.4.3",      "Composite Instance Root Retrieve - GET" },
			{ SOP::CompositeInstanceRetrieveWithoutBulkData_Get,                       "1.2.840.10008.5.1.4.1.2.5.3",      "Composite Instance Retrieve Without Bulk Data - GET" },
			{ SOP::DefinedProcedureProtocol_Find,                                      "1.2.840.10008.5.1.4.20.1",         "Defined Procedure Protocol Information Model - FIND" },
			{ SOP::DefinedProcedureProtocol_Move,                                      "1.2.840.10008.5.1.4.20.2",         "Defined Procedure Protocol Information Model - MOVE" },
			{ SOP::DefinedProcedureProtocol_Get,                                       "1.2.840.10008.5.1.4.20.3",         "Defined Procedure Protocol Information Model - GET" },
			{ SOP::ModalityWorklist_Find,                                              "1.2.840.10008.5.1.4.31",           "Modality Worklist Information Model - FIND" },
			{ SOP::InstanceAvailabilityNotification,                                   "1.2.840.10008.5.1.4.33",           "Instance Availability Notification SOP Class" },
			{ SOP::RTBrachyApplicationSetupDeliveryInstructionStorage,                 "1.2.840.10008.5.1.4.34.10",        "RT Brachy Application Setup Delivery Instruction Storage" },
			{ SOP::UnifiedProcedureStepPush,                                           "1.2.840.10008.5.1.4.34.6.1",       "Unified Procedure Step - Push SOP Class" },
			{ SOP::UnifiedProcedureStepWatch,                                          "1.2.840.10008.5.1.4.34.6.2",       "Unified Procedure Step - Watch SOP Class" },
			{ SOP::UnifiedProcedureStepPull,                                           "1.2.840.10008.5.1.4.34.6.3",       "Unified Procedure Step - Pull SOP Class" },
			{ SOP::UnifiedProcedureStepEvent,                                          "1.2.840.10008.5.1.4.34.6.4",       "Unified Procedure Step - Event SOP Class" },
			{ SOP::RTBeamsDeliveryInstructionStorage,                                  "1.2.840.10008.5.1.4.34.7",         "RT Beams Delivery Instruction Storage" },
			{ SOP::RTConventionalMachineVerification,                                  "1.2.840.10008.5.1.4.34.8",         "RT Conventional Machine Verification" },
			{ SOP::RTIonMachineVerification,                                           "1.2.840.10008.5.1.4.34.9",         "RT Ion Machine Verification" },
			{ SOP::GeneralRelevantPatientInformationQuery,                             "1.2.840.10008.5.1.4.37.1",         "General Relevant Patient Information Query" },
			{ SOP::BreastImagingRelevantPatientInformationQuery,                       "1.2.840.10008.5.1.4.37.2",         "Breast Imaging Relevant Patient Information Query" },
			{ SOP::CardiacRelevantPatientInformationQuery,                             "1.2.840.10008.5.1.4.37.3",         "Cardiac Relevant Patient Information Query" },
			{ SOP::HangingProtocolStorage,                                             "1.2.840.10008.5.1.4.38.1",         "Hanging Protocol Storage" },
			{ SOP::HangingProtocol_Find,                                               "1.2.840.10008.5.1.4.38.2",         "Hanging Protocol Information Model - FIND" },
			{ SOP::HangingProtocol_Move,                                               "1.2.840.10008.5.1.4.38.3",         "Hanging Protocol Information Model - MOVE" },
			{ SOP::HangingProtocol_Get,                                                "1.2.840.10008.5.1.4.38.4",         "Hanging Protocol Information Model - GET" },
			{ SOP::ColorPaletteQR_Find,                                                "1.2.840.10008.5.1.4.39.2",         "Color Palette Query/Retrieve Information Model - FIND" },
			{ SOP::ColorPaletteQR_Move,                                                "1.2.840.10008.5.1.4.39.3",         "Color Palette Query/Retrieve Information Model - MOVE" },
			{ SOP::ColorPaletteQR_Get,                                                 "1.2.840.10008.5.1.4.39.4",         "Color Palette Query/Retrieve Information Model - GET" },
			{ SOP::ProductCharacteristicsQuery,                                        "1.2.840.10008.5.1.4.41",           "Product Characteristics Query SOP Class" },
			{ SOP::SubstanceApprovalQuery,                                             "1.2.840.10008.5.1.4.42",           "Substance Approval Query SOP Class" },
			{ SOP::GenericImplantTemplateStorage,                                      "1.2.840.10008.5.1.4.43.1",         "Generic Implant Template Storage" },
			{ SOP::GenericImplantTemplate_Find,                                        "1.2.840.10008.5.1.4.43.2",         "Generic Implant Template Information Model - FIND" },
			{ SOP::GenericImplantTemplate_Move,                                        "1.2.840.10008.5.1.4.43.3",         "Generic Implant Template Information Model - MOVE" },
			{ SOP::GenericImplantTemplate_Get,                                         "1.2.840.10008.5.1.4.43.4",         "Generic Implant Template Information Model - GET" },
			{ SOP::ImplantAssemblyTemplateStorage,                                     "1.2.840.10008.5.1.4.44.1",         "Implant Assembly Template Storage" },
			{ SOP::ImplantAssemblyTemplate_Find,                                       "1.2.840.10008.5.1.4.44.2",         "Implant Assembly Template Information Model - FIND" },
			{ SOP::ImplantAssemblyTemplate_Move,                                       "1.2.840.10008.5.1.4.44.3",         "Implant Assembly Template Information Model - MOVE" },
			{ SOP::ImplantAssemblyTemplate_Get,                                        "1.2.840.10008.5.1.4.44.4",         "Implant Assembly Template Information Model - GET" },
			{ SOP::ImplantTemplateGroupStorage,                                        "1.2.840.10008.5.1.4.45.1",         "Implant Template Group Storage" },
			{ SOP::ImplantTemplateGroup_Find,                                          "1.2.840.10008.5.1.4.45.2",         "Implant Template Group Information Model - FIND" },
			{ SOP::ImplantTemplateGroup_Move,                                          "1.2.840.10008.5.1.4.45.3",         "Implant Template Group Information Model - MOVE" },
			{ SOP::ImplantTemplateGroup_Get,                                           "1.2.840.10008.5.1.4.45.4",         "Implant Template Group Information Model - GET" },
			{ SOP::StorageCommitmentPullModel,                                         "1.2.840.10008.1.20.2",             "Storage Commitment Pull Model SOP Class (Retired)" },
			{ SOP::BasicStudyContentNotification,                                      "1.2.840.10008.1.9",                "Basic Study Content Notification SOP Class (Retired)" },
			{ SOP::DetachedPatientManagement,                                          "1.2.840.10008.3.1.2.1.1",          "Detached Patient Management SOP Class (Retired)" },
			{ SOP::DetachedPatientManagementMeta,                                      "1.2.840.10008.3.1.2.1.4",          "Detached Patient Management Meta SOP Class (Retired)" },
			{ SOP::DetachedVisitManagement,                                            "1.2.840.10008.3.1.2.2.1",          "Detached Visit Management SOP Class (Retired)" },
			{ SOP::DetachedStudyManagement,                                            "1.2.840.10008.3.1.2.3.1",          "Detached Study Management SOP Class (Retired)" },
			{ SOP::StudyComponentManagement,                                           "1.2.840.10008.3.1.2.3.2",          "Study Component Management SOP Class (Retired)" },
			{ SOP::DetachedResultsManagement,                                          "1.2.840.10008.3.1.2.5.1",          "Detached Results Management SOP Class (Retired)" },
			{ SOP::DetachedResultsManagementMeta,                                      "1.2.840.10008.3.1.2.5.4",          "Detached Results Management Meta SOP Class (Retired)" },
			{ SOP::DetachedStudyManagementMeta,                                        "1.2.840.10008.3.1.2.5.5",          "Detached Study Management Meta SOP Class (Retired)" },
			{ SOP::DetachedInterpretationManagement,                                   "1.2.840.10008.3.1.2.6.1",          "Detached Interpretation Management SOP Class (Retired)" },
			{ SOP::ReferencedColorPrintManagementMeta,                                 "1.2.840.10008.5.1.1.18.1",         "Referenced Color Print Management Meta SOP Class (Retired)" },
			{ SOP::ImageOverlayBox,                                                    "1.2.840.10008.5.1.1.24",           "Image Overlay Box SOP Class (Retired)" },
			{ SOP::BasicPrintImageOverlayBox,                                          "1.2.840.10008.5.1.1.24.1",         "Basic Print Image Overlay Box SOP Class (Retired)" },
			{ SOP::PrintQueueManagement,                                               "1.2.840.10008.5.1.1.26",           "Print Queue Management SOP Class (Retired)" },
			{ SOP::StoredPrintStorage,                                                 "1.2.840.10008.5.1.1.27",           "Stored Print Storage SOP Class (Retired)" },
			{ SOP::HardcopyGrayscaleImageStorage,                                      "1.2.840.10008.5.1.1.29",           "Hardcopy Grayscale Image Storage SOP Class (Retired)" },
			{ SOP::HardcopyColorImageStorage,                                          "1.2.840.10008.5.1.1.30",           "Hardcopy Color Image Storage SOP Class (Retired)" },
			{ SOP::PullPrintRequest,                                                   "1.2.840.10008.5.1.1.31",           "Pull Print Request SOP Class (Retired)" },
			{ SOP::PullStoredPrintManagementMeta,                                      "1.2.840.10008.5.1.1.32",           "Pull Stored Print Management Meta SOP Class (Retired)" },
			{ SOP::ReferencedImageBox,                                                 "1.2.840.10008.5.1.1.4.2",          "Referenced Image Box SOP Class (Retired)" },
			{ SOP::ReferencedGrayscalePrintManagementMeta,                             "1.2.840.10008.5.1.1.9.1",          "Referenced Grayscale Print Management Meta SOP Class (Retired)" },
			{ SOP::StandaloneModalityLUTStorage,                                       "1.2.840.10008.5.1.4.1.1.10",       "Standalone Modality LUT Storage (Retired)" },
			{ SOP::StandaloneVOILUTStorage,                                            "1.2.840.10008.5.1.4.1.1.11",       "Standalone VOI LUT Storage (Retired)" },
			{ SOP::XRayAngiographicBiPlaneImageStorage,                                "1.2.840.10008.5.1.4.1.1.12.3",     "X-Ray Angiographic Bi-Plane Image Storage (Retired)" },
			{ SOP::StandalonePETCurveStorage,                                          "1.2.840.10008.5.1.4.1.1.129",      "Standalone PET Curve Storage (Retired)" },
			{ SOP::UltrasoundMultiframeImageStorage_Retired,                           "1.2.840.10008.5.1.4.1.1.3",        "Ultrasound Multi-frame Image Storage (Retired)" },
			{ SOP::NuclearMedicineImageStorage_Retired,                                "1.2.840.10008.5.1.4.1.1.5",        "Nuclear Medicine Image Storage (Retired)" },
			{ SOP::UltrasoundImageStorage_Retired,                                     "1.2.840.10008.5.1.4.1.1.6",        "Ultrasound Image Storage (Retired)" },
			{ SOP::VLImageStorageTrial,                                                "1.2.840.10008.5.1.4.1.1.77.1",     "VL Image Storage - Trial (Retired)" },
			{ SOP::VLMultiframeImageStorageTrial,                                      "1.2.840.10008.5.1.4.1.1.77.2",     "VL Multi-frame Image Storage - Trial (Retired)" },
			{ SOP::StandaloneOverlayStorage,                                           "1.2.840.10008.5.1.4.1.1.8",        "Standalone Overlay Storage (Retired)" },
			{ SOP::TextSRStorageTrial,                                                 "1.2.840.10008.5.1.4.1.1.88.1",     "Text SR Storage - Trial (Retired)" },
			{ SOP::AudioSRStorageTrial,                                                "1.2.840.10008.5.1.4.1.1.88.2",     "Audio SR Storage - Trial (Retired)" },
			{ SOP::DetailSRStorageTrial,                                               "1.2.840.10008.5.1.4.1.1.88.3",     "Detail SR Storage - Trial (Retired)" },
			{ SOP::ComprehensiveSRStorageTrial,                                        "1.2.840.10008.5.1.4.1.1.88.4",     "Comprehensive SR Storage - Trial (Retired)" },
			{ SOP::StandaloneCurveStorage,                                             "1.2.840.10008.5.1.4.1.1.9",        "Standalone Curve Storage (Retired)" },
			{ SOP::WaveformStorageTrial,                                               "1.2.840.10008.5.1.4.1.1.9.1",      "Waveform Storage - Trial (Retired)" },
			{ SOP::PatientStudyOnlyQR_Find,                                            "1.2.840.10008.5.1.4.1.2.3.1",      "Patient/Study Only Query/Retrieve Information Model - FIND (Retired)" },
			{ SOP::PatientStudyOnlyQR_Move,                                            "1.2.840.10008.5.1.4.1.2.3.2",      "Patient/Study Only Query/Retrieve Information Model - MOVE (Retired)" },
			{ SOP::PatientStudyOnlyQR_Get,                                             "1.2.840.10008.5.1.4.1.2.3.3",      "Patient/Study Only Query/Retrieve Information Model - GET (Retired)" },
			{ SOP::GeneralPurposeWorklistManagementMeta,                               "1.2.840.10008.5.1.4.32",           "General Purpose Worklist Management Meta SOP Class (Retired)" },
			{ SOP::GeneralPurposeWorklist_Find,                                        "1.2.840.10008.5.1.4.32.1",         "General Purpose Worklist Information Model - FIND (Retired)" },
			{ SOP::GeneralPurposeScheduledProcedureStep,                               "1.2.840.10008.5.1.4.32.2",         "General Purpose Scheduled Procedure Step SOP Class (Retired)" },
			{ SOP::GeneralPurposePerformedProcedureStep,                               "1.2.840.10008.5.1.4.32.3",         "General Purpose Performed Procedure Step SOP Class (Retired)" },
			{ SOP::RTBeamsDeliveryInstructionStorageTrial,                             "1.2.840.10008.5.1.4.34.1",         "RT Beams Delivery Instruction Storage - Trial (Retired)" },
			{ SOP::RTConventionalMachineVerificationTrial,                             "1.2.840.10008.5.1.4.34.2",         "RT Conventional Machine Verification - Trial (Retired)" },
			{ SOP::RTIonMachineVerificationTrial,                                      "1.2.840.10008.5.1.4.34.3",         "RT Ion Machine Verification - Trial (Retired)" },
			{ SOP::UnifiedProcedureStepPushTrial,                                      "1.2.840.10008.5.1.4.34.4.1",       "Unified Procedure Step - Push SOP Class - Trial (Retired)" },
			{ SOP::UnifiedProcedureStepWatchTrial,                                     "1.2.840.10008.5.1.4.34.4.2",       "Unified Procedure Step - Watch SOP Class - Trial (Retired)" },
			{ SOP::UnifiedProcedureStepPullTrial,                                      "1.2.840.10008.5.1.4.34.4.3",       "Unified Procedure Step - Pull SOP Class - Trial (Retired)" },
			{ SOP::UnifiedProcedureStepEventTrial,                                     "1.2.840.10008.5.1.4.34.4.4",       "Unified Procedure Step - Event SOP Class - Trial (Retired)" },
		};

		return sops;
	}

	using SOPClassMap = boost::container::flat_map<std::string, emdl::SOP_Class>;
	const SOPClassMap& getSOPClassMap()
	{
		static SOPClassMap sopClassMap = [] {
			SOPClassMap sopcm;
			const auto& sopcp = getSOPClassStructs();
			sopcm.reserve(sopcp.size());
			for (const auto& sop : sopcp)
				sopcm[sop.uid] = sop.sopClass;
			return sopcm;
		}();

		return sopClassMap;
	}

	using SOPClasses = std::vector<emdl::SOP_Class>;
	const SOPClasses& getStorageClasses()
	{
		using SOP = emdl::SOP_Class;
		static SOPClasses storageClasses = {
			SOP::MediaStorageDirectoryStorage,
			SOP::StoredPrintStorage,
			SOP::HardcopyGrayscaleImageStorage,
			SOP::HardcopyColorImageStorage,
			SOP::ComputedRadiographyImageStorage,
			SOP::DigitalXRayImageStorageForPresentation,
			SOP::DigitalXRayImageStorageForProcessing,
			SOP::DigitalMammographyXRayImageStorageForPresentation,
			SOP::DigitalMammographyXRayImageStorageForProcessing,
			SOP::DigitalIntraOralXRayImageStorageForPresentation,
			SOP::DigitalIntraOralXRayImageStorageForProcessing,
			SOP::StandaloneModalityLUTStorage,
			SOP::EncapsulatedPDFStorage,
			SOP::EncapsulatedCDAStorage,
			SOP::StandaloneVOILUTStorage,
			SOP::GrayscaleSoftcopyPresentationStateStorage,
			SOP::ColorSoftcopyPresentationStateStorage,
			SOP::PseudoColorSoftcopyPresentationStateStorage,
			SOP::BlendingSoftcopyPresentationStateStorage,
			SOP::XAXRFGrayscaleSoftcopyPresentationStateStorage,
			SOP::GrayscalePlanarMPRVolumetricPresentationStateStorage,
			SOP::CompositingPlanarMPRVolumetricPresentationStateStorage,
			SOP::XRayAngiographicImageStorage,
			SOP::EnhancedXAImageStorage,
			SOP::XRayRadiofluoroscopicImageStorage,
			SOP::EnhancedXRFImageStorage,
			SOP::XRayAngiographicBiPlaneImageStorage,
			SOP::PositronEmissionTomographyImageStorage,
			SOP::LegacyConvertedEnhancedPETImageStorage,
			SOP::StandalonePETCurveStorage,
			SOP::XRay3DAngiographicImageStorage,
			SOP::XRay3DCraniofacialImageStorage,
			SOP::BreastTomosynthesisImageStorage,
			SOP::BreastProjectionXRayImageStorageForPresentation,
			SOP::BreastProjectionXRayImageStorageForProcessing,
			SOP::EnhancedPETImageStorage,
			SOP::BasicStructuredDisplayStorage,
			SOP::IntravascularOpticalCoherenceTomographyImageStorageForPresentation,
			SOP::IntravascularOpticalCoherenceTomographyImageStorageForProcessing,
			SOP::CTImageStorage,
			SOP::EnhancedCTImageStorage,
			SOP::LegacyConvertedEnhancedCTImageStorage,
			SOP::NuclearMedicineImageStorage,
			SOP::CTDefinedProcedureProtocolStorage,
			SOP::CTPerformedProcedureProtocolStorage,
			SOP::UltrasoundMultiframeImageStorage_Retired,
			SOP::UltrasoundMultiframeImageStorage,
			SOP::ParametricMapStorage,
			SOP::MRImageStorage,
			SOP::EnhancedMRImageStorage,
			SOP::MRSpectroscopyStorage,
			SOP::EnhancedMRColorImageStorage,
			SOP::LegacyConvertedEnhancedMRImageStorage,
			SOP::RTImageStorage,
			SOP::RTDoseStorage,
			SOP::RTStructureSetStorage,
			SOP::RTBeamsTreatmentRecordStorage,
			SOP::RTPlanStorage,
			SOP::RTBrachyTreatmentRecordStorage,
			SOP::RTTreatmentSummaryRecordStorage,
			SOP::RTIonPlanStorage,
			SOP::RTIonBeamsTreatmentRecordStorage,
			SOP::NuclearMedicineImageStorage_Retired,
			SOP::DICOSCTImageStorage,
			SOP::DICOSDigitalXRayImageStorageForPresentation,
			SOP::DICOSDigitalXRayImageStorageForProcessing,
			SOP::DICOSThreatDetectionReportStorage,
			SOP::DICOS2DAITStorage,
			SOP::DICOS3DAITStorage,
			SOP::DICOSQuadrupoleResonanceQRStorage,
			SOP::UltrasoundImageStorage_Retired,
			SOP::UltrasoundImageStorage,
			SOP::EnhancedUSVolumeStorage,
			SOP::EddyCurrentImageStorage,
			SOP::EddyCurrentMultiframeImageStorage,
			SOP::RawDataStorage,
			SOP::SpatialRegistrationStorage,
			SOP::SpatialFiducialsStorage,
			SOP::DeformableSpatialRegistrationStorage,
			SOP::SegmentationStorage,
			SOP::SurfaceSegmentationStorage,
			SOP::TractographyResultsStorage,
			SOP::RealWorldValueMappingStorage,
			SOP::SurfaceScanMeshStorage,
			SOP::SurfaceScanPointCloudStorage,
			SOP::SecondaryCaptureImageStorage,
			SOP::MultiframeSingleBitSecondaryCaptureImageStorage,
			SOP::MultiframeGrayscaleByteSecondaryCaptureImageStorage,
			SOP::MultiframeGrayscaleWordSecondaryCaptureImageStorage,
			SOP::MultiframeTrueColorSecondaryCaptureImageStorage,
			SOP::VLImageStorageTrial,
			SOP::VLEndoscopicImageStorage,
			SOP::VideoEndoscopicImageStorage,
			SOP::VLMicroscopicImageStorage,
			SOP::VideoMicroscopicImageStorage,
			SOP::VLSlideCoordinatesMicroscopicImageStorage,
			SOP::VLPhotographicImageStorage,
			SOP::VideoPhotographicImageStorage,
			SOP::OphthalmicPhotography8BitImageStorage,
			SOP::OphthalmicPhotography16BitImageStorage,
			SOP::StereometricRelationshipStorage,
			SOP::OphthalmicTomographyImageStorage,
			SOP::WideFieldOphthalmicPhotographyStereographicProjectionImageStorage,
			SOP::WideFieldOphthalmicPhotography3DCoordinatesImageStorage,
			SOP::VLWholeSlideMicroscopyImageStorage,
			SOP::VLMultiframeImageStorageTrial,
			SOP::LensometryMeasurementsStorage,
			SOP::AutorefractionMeasurementsStorage,
			SOP::KeratometryMeasurementsStorage,
			SOP::SubjectiveRefractionMeasurementsStorage,
			SOP::VisualAcuityMeasurementsStorage,
			SOP::SpectaclePrescriptionReportStorage,
			SOP::OphthalmicAxialMeasurementsStorage,
			SOP::IntraocularLensCalculationsStorage,
			SOP::MacularGridThicknessandVolumeReportStorage,
			SOP::StandaloneOverlayStorage,
			SOP::OphthalmicVisualFieldStaticPerimetryMeasurementsStorage,
			SOP::OphthalmicThicknessMapStorage,
			SOP::CornealTopographyMapStorage,
			SOP::TextSRStorageTrial,
			SOP::BasicTextSRStorage,
			SOP::AudioSRStorageTrial,
			SOP::EnhancedSRStorage,
			SOP::DetailSRStorageTrial,
			SOP::ComprehensiveSRStorage,
			SOP::Comprehensive3DSRStorage,
			SOP::ExtensibleSRStorage,
			SOP::ComprehensiveSRStorageTrial,
			SOP::ProcedureLogStorage,
			SOP::MammographyCADSRStorage,
			SOP::KeyObjectSelectionDocumentStorage,
			SOP::ChestCADSRStorage,
			SOP::XRayRadiationDoseSRStorage,
			SOP::RadiopharmaceuticalRadiationDoseSRStorage,
			SOP::ColonCADSRStorage,
			SOP::ImplantationPlanSRStorage,
			SOP::AcquisitionContextSRStorage,
			SOP::SimplifiedAdultEchoSRStorage,
			SOP::StandaloneCurveStorage,
			SOP::WaveformStorageTrial,
			SOP::TwelveLeadECGWaveformStorage,
			SOP::GeneralECGWaveformStorage,
			SOP::AmbulatoryECGWaveformStorage,
			SOP::HemodynamicWaveformStorage,
			SOP::CardiacElectrophysiologyWaveformStorage,
			SOP::BasicVoiceAudioWaveformStorage,
			SOP::GeneralAudioWaveformStorage,
			SOP::ArterialPulseWaveformStorage,
			SOP::RespiratoryWaveformStorage,
			SOP::ContentAssessmentResultsStorage,
			SOP::RTBeamsDeliveryInstructionStorageTrial,
			SOP::RTBrachyApplicationSetupDeliveryInstructionStorage,
			SOP::RTBeamsDeliveryInstructionStorage,
			SOP::HangingProtocolStorage,
			SOP::GenericImplantTemplateStorage,
			SOP::ImplantAssemblyTemplateStorage,
			SOP::ImplantTemplateGroupStorage
		};

		return storageClasses;
	}

	const SOPClasses& getStructuredReportsClasses()
	{
		using SOP = emdl::SOP_Class;
		static SOPClasses structuredReports = {
			SOP::DICOSThreatDetectionReportStorage,
			SOP::SpectaclePrescriptionReportStorage,
			SOP::MacularGridThicknessandVolumeReportStorage,
			SOP::TextSRStorageTrial,
			SOP::BasicTextSRStorage,
			SOP::AudioSRStorageTrial,
			SOP::EnhancedSRStorage,
			SOP::DetailSRStorageTrial,
			SOP::ComprehensiveSRStorage,
			SOP::Comprehensive3DSRStorage,
			SOP::ExtensibleSRStorage,
			SOP::ComprehensiveSRStorageTrial,
			SOP::ProcedureLogStorage,
			SOP::MammographyCADSRStorage,
			SOP::KeyObjectSelectionDocumentStorage,
			SOP::ChestCADSRStorage,
			SOP::XRayRadiationDoseSRStorage,
			SOP::RadiopharmaceuticalRadiationDoseSRStorage,
			SOP::ColonCADSRStorage,
			SOP::ImplantationPlanSRStorage,
			SOP::AcquisitionContextSRStorage,
			SOP::SimplifiedAdultEchoSRStorage
		};

		return structuredReports;
	}

	const SOPClasses& getPresentationStatesClasses()
	{
		using SOP = emdl::SOP_Class;
		static SOPClasses presentationStates = {
			SOP::StandaloneVOILUTStorage,
			SOP::GrayscaleSoftcopyPresentationStateStorage,
			SOP::ColorSoftcopyPresentationStateStorage,
			SOP::PseudoColorSoftcopyPresentationStateStorage,
			SOP::BlendingSoftcopyPresentationStateStorage,
			SOP::XAXRFGrayscaleSoftcopyPresentationStateStorage,
			SOP::GrayscalePlanarMPRVolumetricPresentationStateStorage,
			SOP::CompositingPlanarMPRVolumetricPresentationStateStorage
		};

		return presentationStates;
	}

	const SOPClasses& getRetiredClasses()
	{
		using SOP = emdl::SOP_Class;
		static SOPClasses retired = {
			SOP::StorageCommitmentPullModel,
			SOP::BasicStudyContentNotification,
			SOP::DetachedPatientManagement,
			SOP::DetachedPatientManagementMeta,
			SOP::DetachedVisitManagement,
			SOP::DetachedStudyManagement,
			SOP::StudyComponentManagement,
			SOP::DetachedResultsManagement,
			SOP::DetachedResultsManagementMeta,
			SOP::DetachedStudyManagementMeta,
			SOP::DetachedInterpretationManagement,
			SOP::ReferencedColorPrintManagementMeta,
			SOP::ImageOverlayBox,
			SOP::BasicPrintImageOverlayBox,
			SOP::PrintQueueManagement,
			SOP::PullPrintRequest,
			SOP::PullStoredPrintManagementMeta,
			SOP::ReferencedImageBox,
			SOP::ReferencedGrayscalePrintManagementMeta,
			SOP::PatientStudyOnlyQR_Find,
			SOP::PatientStudyOnlyQR_Move,
			SOP::PatientStudyOnlyQR_Get,
			SOP::GeneralPurposeWorklistManagementMeta,
			SOP::GeneralPurposeWorklist_Find,
			SOP::GeneralPurposeScheduledProcedureStep,
			SOP::GeneralPurposePerformedProcedureStep,
			SOP::RTConventionalMachineVerificationTrial,
			SOP::RTIonMachineVerificationTrial,
			SOP::UnifiedProcedureStepPushTrial,
			SOP::UnifiedProcedureStepWatchTrial,
			SOP::UnifiedProcedureStepPullTrial,
			SOP::UnifiedProcedureStepEventTrial,
			SOP::StoredPrintStorage,
			SOP::HardcopyGrayscaleImageStorage,
			SOP::HardcopyColorImageStorage,
			SOP::StandaloneModalityLUTStorage,
			SOP::StandaloneVOILUTStorage,
			SOP::XRayAngiographicBiPlaneImageStorage,
			SOP::StandalonePETCurveStorage,
			SOP::UltrasoundMultiframeImageStorage_Retired,
			SOP::NuclearMedicineImageStorage_Retired,
			SOP::UltrasoundImageStorage_Retired,
			SOP::VLImageStorageTrial,
			SOP::VLMultiframeImageStorageTrial,
			SOP::StandaloneOverlayStorage,
			SOP::TextSRStorageTrial,
			SOP::AudioSRStorageTrial,
			SOP::DetailSRStorageTrial,
			SOP::ComprehensiveSRStorageTrial,
			SOP::StandaloneCurveStorage,
			SOP::WaveformStorageTrial,
			SOP::RTBeamsDeliveryInstructionStorageTrial
		};

		return retired;
	}

	using BitSet = std::vector<bool>;
	const BitSet& getStorageBitSet()
	{
		static BitSet storageBitSet = [] {
			BitSet bitSet;
			bitSet.assign(getSOPClassStructs().size(), false);
			for (auto sop : getStorageClasses())
				bitSet[static_cast<int>(sop)] = true;
			return bitSet;
		}();

		return storageBitSet;
	}

	const BitSet& getStructuredReportsBitSet()
	{
		static BitSet structuredReportsBitSet = [] {
			BitSet bitSet;
			bitSet.assign(getSOPClassStructs().size(), false);
			for (auto sop : getStructuredReportsClasses())
				bitSet[static_cast<int>(sop)] = true;
			return bitSet;
		}();

		return structuredReportsBitSet;
	}

	const BitSet& getPresentationStatesBitSet()
	{
		static BitSet presentationStatesBitSet = [] {
			BitSet bitSet;
			bitSet.assign(getSOPClassStructs().size(), false);
			for (auto sop : getPresentationStatesClasses())
				bitSet[static_cast<int>(sop)] = true;
			return bitSet;
		}();

		return presentationStatesBitSet;
	}

	const BitSet& getRetiredBitSet()
	{
		static BitSet retiredStatesBitSet = [] {
			BitSet bitSet;
			bitSet.assign(getSOPClassStructs().size(), false);
			for (auto sop : getRetiredClasses())
				bitSet[static_cast<int>(sop)] = true;
			return bitSet;
		}();

		return retiredStatesBitSet;
	}

	// To be verified: if it is a storage class, but not a SR nor a PR, is it always an image?
	const BitSet& getImagesBitSet()
	{
		static BitSet imagesBitSet = [] {
			BitSet bitSet = getStorageBitSet();
			for (auto sop : getStructuredReportsClasses())
				bitSet[static_cast<int>(sop)] = false;
			for (auto sop : getPresentationStatesClasses())
				bitSet[static_cast<int>(sop)] = false;
			return bitSet;
		}();

		return imagesBitSet;
	}

}

namespace emdl
{

	SOP_Class getSOPClass(const std::string& uid)
	{
		const auto& sopMap = getSOPClassMap();
		auto it = sopMap.find(uid);
		if (it != sopMap.end())
			return it->second;
		return SOP_Class::Unknown;
	}

	const std::string& getSOPClassUID(SOP_Class sop_class)
	{
		const auto& sops = getSOPClassStructs();
		int index = static_cast<int>(sop_class);
		return sops[index].uid;
	}

	const std::string& getSOPClassDescription(SOP_Class sop_class)
	{
		const auto& sops = getSOPClassStructs();
		int index = static_cast<int>(sop_class);
		return sops[index].description;
	}

	std::vector<SOP_Class> getSOPClassList()
	{
		static const auto list = [] {
			std::vector<SOP_Class> list;
			const auto& sopcp = getSOPClassStructs();
			list.reserve(sopcp.size());
			for (const auto& sop : sopcp)
				list.push_back(sop.sopClass);
			return list;
		}();
		return list;
	}

	bool isStorage(SOP_Class sop)
	{
		int index = static_cast<int>(sop);
		return getStorageBitSet()[index];
	}

	bool isImage(SOP_Class sop)
	{
		int index = static_cast<int>(sop);
		return getImagesBitSet()[index];
	}

	bool isStructuredReport(SOP_Class sop)
	{
		int index = static_cast<int>(sop);
		return getStructuredReportsBitSet()[index];
	}

	bool isPresentationState(SOP_Class sop)
	{
		int index = static_cast<int>(sop);
		return getPresentationStatesBitSet()[index];
	}

	bool isRetired(SOP_Class sop)
	{
		int index = static_cast<int>(sop);
		return getRetiredBitSet()[index];
	}

}
