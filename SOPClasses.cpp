#include "SOPClasses.h"

#include <boost/container/flat_map.hpp>

#include <algorithm>
#include <vector>

namespace
{

	using SOPClassPair = std::pair<emdl::SOP_Class, std::string>;
	using SOPClassPairs = std::vector<SOPClassPair>;
	const SOPClassPairs& getSOPClassPairs()
	{
		using SOP = emdl::SOP_Class;
		static SOPClassPairs sops = {
			{ SOP::Unknown,                                                            "Not supported" },
			{ SOP::Verification,                                                       "1.2.840.10008.1.1" },
			{ SOP::StorageCommitmentPushModel,                                         "1.2.840.10008.1.20.1" },
			{ SOP::StorageCommitmentPullModel,                                         "1.2.840.10008.1.20.2" },
			{ SOP::ProceduralEventLogging,                                             "1.2.840.10008.1.40" },
			{ SOP::SubstanceAdministrationLogging,                                     "1.2.840.10008.1.42" },
			{ SOP::BasicStudyContentNotification,                                      "1.2.840.10008.1.9" },
			{ SOP::DetachedPatientManagement,                                          "1.2.840.10008.3.1.2.1.1" },
			{ SOP::DetachedPatientManagementMeta,                                      "1.2.840.10008.3.1.2.1.4" },
			{ SOP::DetachedVisitManagement,                                            "1.2.840.10008.3.1.2.2.1" },
			{ SOP::DetachedStudyManagement,                                            "1.2.840.10008.3.1.2.3.1" },
			{ SOP::StudyComponentManagement,                                           "1.2.840.10008.3.1.2.3.2" },
			{ SOP::ModalityPerformedProcedureStep,                                     "1.2.840.10008.3.1.2.3.3" },
			{ SOP::ModalityPerformedProcedureStepRetrieve,                             "1.2.840.10008.3.1.2.3.4" },
			{ SOP::ModalityPerformedProcedureStepNotification,                         "1.2.840.10008.3.1.2.3.5" },
			{ SOP::DetachedResultsManagement,                                          "1.2.840.10008.3.1.2.5.1" },
			{ SOP::DetachedResultsManagementMeta,                                      "1.2.840.10008.3.1.2.5.4" },
			{ SOP::DetachedStudyManagementMeta,                                        "1.2.840.10008.3.1.2.5.5" },
			{ SOP::DetachedInterpretationManagement,                                   "1.2.840.10008.3.1.2.6.1" },
			{ SOP::BasicFilmSession,                                                   "1.2.840.10008.5.1.1.1" },
			{ SOP::PrintJob,                                                           "1.2.840.10008.5.1.1.14" },
			{ SOP::BasicAnnotationBox,                                                 "1.2.840.10008.5.1.1.15" },
			{ SOP::Printer,                                                            "1.2.840.10008.5.1.1.16" },
			{ SOP::PrinterConfigurationRetrieval,                                      "1.2.840.10008.5.1.1.16.376" },
			{ SOP::BasicColorPrintManagementMeta,                                      "1.2.840.10008.5.1.1.18" },
			{ SOP::ReferencedColorPrintManagementMeta,                                 "1.2.840.10008.5.1.1.18.1" },
			{ SOP::BasicFilmBox,                                                       "1.2.840.10008.5.1.1.2" },
			{ SOP::VOILUTBox,                                                          "1.2.840.10008.5.1.1.22" },
			{ SOP::PresentationLUT,                                                    "1.2.840.10008.5.1.1.23" },
			{ SOP::ImageOverlayBox,                                                    "1.2.840.10008.5.1.1.24" },
			{ SOP::BasicPrintImageOverlayBox,                                          "1.2.840.10008.5.1.1.24.1" },
			{ SOP::PrintQueueManagement,                                               "1.2.840.10008.5.1.1.26" },
			{ SOP::PullPrintRequest,                                                   "1.2.840.10008.5.1.1.31" },
			{ SOP::PullStoredPrintManagementMeta,                                      "1.2.840.10008.5.1.1.32" },
			{ SOP::MediaCreationManagementUID,                                         "1.2.840.10008.5.1.1.33" },
			{ SOP::BasicGrayscaleImageBox,                                             "1.2.840.10008.5.1.1.4" },
			{ SOP::BasicColorImageBox,                                                 "1.2.840.10008.5.1.1.4.1" },
			{ SOP::ReferencedImageBox,                                                 "1.2.840.10008.5.1.1.4.2" },
			{ SOP::DisplaySystem,                                                      "1.2.840.10008.5.1.1.40" },
			{ SOP::BasicGrayscalePrintManagementMeta,                                  "1.2.840.10008.5.1.1.9" },
			{ SOP::ReferencedGrayscalePrintManagementMeta,                             "1.2.840.10008.5.1.1.9.1" },
			{ SOP::PatientRootQR_Find,                                                 "1.2.840.10008.5.1.4.1.2.1.1" },
			{ SOP::PatientRootQR_Move,                                                 "1.2.840.10008.5.1.4.1.2.1.2" },
			{ SOP::PatientRootQR_Get,                                                  "1.2.840.10008.5.1.4.1.2.1.3" },
			{ SOP::StudyRootQR_Find,                                                   "1.2.840.10008.5.1.4.1.2.2.1" },
			{ SOP::StudyRootQR_Move,                                                   "1.2.840.10008.5.1.4.1.2.2.2" },
			{ SOP::StudyRootQR_Get,                                                    "1.2.840.10008.5.1.4.1.2.2.3" },
			{ SOP::PatientStudyOnlyQR_Find,                                            "1.2.840.10008.5.1.4.1.2.3.1" },
			{ SOP::PatientStudyOnlyQR_Move,                                            "1.2.840.10008.5.1.4.1.2.3.2" },
			{ SOP::PatientStudyOnlyQR_Get,                                             "1.2.840.10008.5.1.4.1.2.3.3" },
			{ SOP::CompositeInstanceRootRetrieve_Move,                                 "1.2.840.10008.5.1.4.1.2.4.2" },
			{ SOP::CompositeInstanceRootRetrieve_Get,                                  "1.2.840.10008.5.1.4.1.2.4.3" },
			{ SOP::CompositeInstanceRetrieveWithoutBulkData_Get,                       "1.2.840.10008.5.1.4.1.2.5.3" },
			{ SOP::DefinedProcedureProtocol_Find,                                      "1.2.840.10008.5.1.4.20.1" },
			{ SOP::DefinedProcedureProtocol_Move,                                      "1.2.840.10008.5.1.4.20.2" },
			{ SOP::DefinedProcedureProtocol_Get,                                       "1.2.840.10008.5.1.4.20.3" },
			{ SOP::ModalityWorklist_Find,                                              "1.2.840.10008.5.1.4.31" },
			{ SOP::GeneralPurposeWorklistManagementMeta,                               "1.2.840.10008.5.1.4.32" },
			{ SOP::GeneralPurposeWorklist_Find,                                        "1.2.840.10008.5.1.4.32.1" },
			{ SOP::GeneralPurposeScheduledProcedureStep,                               "1.2.840.10008.5.1.4.32.2" },
			{ SOP::GeneralPurposePerformedProcedureStep,                               "1.2.840.10008.5.1.4.32.3" },
			{ SOP::InstanceAvailabilityNotification,                                   "1.2.840.10008.5.1.4.33" },
			{ SOP::RTConventionalMachineVerificationTrial,                             "1.2.840.10008.5.1.4.34.2" },
			{ SOP::RTIonMachineVerificationTrial,                                      "1.2.840.10008.5.1.4.34.3" },
			{ SOP::UnifiedProcedureStepPushTrial,                                      "1.2.840.10008.5.1.4.34.4.1" },
			{ SOP::UnifiedProcedureStepWatchTrial,                                     "1.2.840.10008.5.1.4.34.4.2" },
			{ SOP::UnifiedProcedureStepPullTrial,                                      "1.2.840.10008.5.1.4.34.4.3" },
			{ SOP::UnifiedProcedureStepEventTrial,                                     "1.2.840.10008.5.1.4.34.4.4" },
			{ SOP::UnifiedProcedureStepPush,                                           "1.2.840.10008.5.1.4.34.6.1" },
			{ SOP::UnifiedProcedureStepWatch,                                          "1.2.840.10008.5.1.4.34.6.2" },
			{ SOP::UnifiedProcedureStepPull,                                           "1.2.840.10008.5.1.4.34.6.3" },
			{ SOP::UnifiedProcedureStepEvent,                                          "1.2.840.10008.5.1.4.34.6.4" },
			{ SOP::RTConventionalMachineVerification,                                  "1.2.840.10008.5.1.4.34.8" },
			{ SOP::RTIonMachineVerification,                                           "1.2.840.10008.5.1.4.34.9" },
			{ SOP::GeneralRelevantPatientInformationQuery,                             "1.2.840.10008.5.1.4.37.1" },
			{ SOP::BreastImagingRelevantPatientInformationQuery,                       "1.2.840.10008.5.1.4.37.2" },
			{ SOP::CardiacRelevantPatientInformationQuery,                             "1.2.840.10008.5.1.4.37.3" },
			{ SOP::HangingProtocol_Find,                                               "1.2.840.10008.5.1.4.38.2" },
			{ SOP::HangingProtocol_Move,                                               "1.2.840.10008.5.1.4.38.3" },
			{ SOP::HangingProtocol_Get,                                                "1.2.840.10008.5.1.4.38.4" },
			{ SOP::ColorPaletteQR_Find,                                                "1.2.840.10008.5.1.4.39.2" },
			{ SOP::ColorPaletteQR_Move,                                                "1.2.840.10008.5.1.4.39.3" },
			{ SOP::ColorPaletteQR_Get,                                                 "1.2.840.10008.5.1.4.39.4" },
			{ SOP::ProductCharacteristicsQuery,                                        "1.2.840.10008.5.1.4.41" },
			{ SOP::SubstanceApprovalQuery,                                             "1.2.840.10008.5.1.4.42" },
			{ SOP::GenericImplantTemplate_Find,                                        "1.2.840.10008.5.1.4.43.2" },
			{ SOP::GenericImplantTemplate_Move,                                        "1.2.840.10008.5.1.4.43.3" },
			{ SOP::GenericImplantTemplate_Get,                                         "1.2.840.10008.5.1.4.43.4" },
			{ SOP::ImplantAssemblyTemplate_Find,                                       "1.2.840.10008.5.1.4.44.2" },
			{ SOP::ImplantAssemblyTemplate_Move,                                       "1.2.840.10008.5.1.4.44.3" },
			{ SOP::ImplantAssemblyTemplate_Get,                                        "1.2.840.10008.5.1.4.44.4" },
			{ SOP::ImplantTemplateGroup_Find,                                          "1.2.840.10008.5.1.4.45.2" },
			{ SOP::ImplantTemplateGroup_Move,                                          "1.2.840.10008.5.1.4.45.3" },
			{ SOP::ImplantTemplateGroup_Get,                                           "1.2.840.10008.5.1.4.45.4" },
			{ SOP::MediaStorageDirectoryStorage,                                       "1.2.840.10008.1.3.10" },
			{ SOP::StoredPrintStorage,                                                 "1.2.840.10008.5.1.1.27" },
			{ SOP::HardcopyGrayscaleImageStorage,                                      "1.2.840.10008.5.1.1.29" },
			{ SOP::HardcopyColorImageStorage,                                          "1.2.840.10008.5.1.1.30" },
			{ SOP::ComputedRadiographyImageStorage,                                    "1.2.840.10008.5.1.4.1.1.1" },
			{ SOP::DigitalXRayImageStorageForPresentation,                             "1.2.840.10008.5.1.4.1.1.1.1" },
			{ SOP::DigitalXRayImageStorageForProcessing,                               "1.2.840.10008.5.1.4.1.1.1.1.1" },
			{ SOP::DigitalMammographyXRayImageStorageForPresentation,                  "1.2.840.10008.5.1.4.1.1.1.2" },
			{ SOP::DigitalMammographyXRayImageStorageForProcessing,                    "1.2.840.10008.5.1.4.1.1.1.2.1" },
			{ SOP::DigitalIntraOralXRayImageStorageForPresentation,                    "1.2.840.10008.5.1.4.1.1.1.3" },
			{ SOP::DigitalIntraOralXRayImageStorageForProcessing,                      "1.2.840.10008.5.1.4.1.1.1.3.1" },
			{ SOP::StandaloneModalityLUTStorage,                                       "1.2.840.10008.5.1.4.1.1.10" },
			{ SOP::EncapsulatedPDFStorage,                                             "1.2.840.10008.5.1.4.1.1.104.1" },
			{ SOP::EncapsulatedCDAStorage,                                             "1.2.840.10008.5.1.4.1.1.104.2" },
			{ SOP::StandaloneVOILUTStorage,                                            "1.2.840.10008.5.1.4.1.1.11" },
			{ SOP::GrayscaleSoftcopyPresentationStateStorage,                          "1.2.840.10008.5.1.4.1.1.11.1" },
			{ SOP::ColorSoftcopyPresentationStateStorage,                              "1.2.840.10008.5.1.4.1.1.11.2" },
			{ SOP::PseudoColorSoftcopyPresentationStateStorage,                        "1.2.840.10008.5.1.4.1.1.11.3" },
			{ SOP::BlendingSoftcopyPresentationStateStorage,                           "1.2.840.10008.5.1.4.1.1.11.4" },
			{ SOP::XAXRFGrayscaleSoftcopyPresentationStateStorage,                     "1.2.840.10008.5.1.4.1.1.11.5" },
			{ SOP::GrayscalePlanarMPRVolumetricPresentationStateStorage,               "1.2.840.10008.5.1.4.1.1.11.6" },
			{ SOP::CompositingPlanarMPRVolumetricPresentationStateStorage,             "1.2.840.10008.5.1.4.1.1.11.7" },
			{ SOP::XRayAngiographicImageStorage,                                       "1.2.840.10008.5.1.4.1.1.12.1" },
			{ SOP::EnhancedXAImageStorage,                                             "1.2.840.10008.5.1.4.1.1.12.1.1" },
			{ SOP::XRayRadiofluoroscopicImageStorage,                                  "1.2.840.10008.5.1.4.1.1.12.2" },
			{ SOP::EnhancedXRFImageStorage,                                            "1.2.840.10008.5.1.4.1.1.12.2.1" },
			{ SOP::XRayAngiographicBiPlaneImageStorage,                                "1.2.840.10008.5.1.4.1.1.12.3" },
			{ SOP::PositronEmissionTomographyImageStorage,                             "1.2.840.10008.5.1.4.1.1.128" },
			{ SOP::LegacyConvertedEnhancedPETImageStorage,                             "1.2.840.10008.5.1.4.1.1.128.1" },
			{ SOP::StandalonePETCurveStorage,                                          "1.2.840.10008.5.1.4.1.1.129" },
			{ SOP::XRay3DAngiographicImageStorage,                                     "1.2.840.10008.5.1.4.1.1.13.1.1" },
			{ SOP::XRay3DCraniofacialImageStorage,                                     "1.2.840.10008.5.1.4.1.1.13.1.2" },
			{ SOP::BreastTomosynthesisImageStorage,                                    "1.2.840.10008.5.1.4.1.1.13.1.3" },
			{ SOP::BreastProjectionXRayImageStorageForPresentation,                    "1.2.840.10008.5.1.4.1.1.13.1.4" },
			{ SOP::BreastProjectionXRayImageStorageForProcessing,                      "1.2.840.10008.5.1.4.1.1.13.1.5" },
			{ SOP::EnhancedPETImageStorage,                                            "1.2.840.10008.5.1.4.1.1.130" },
			{ SOP::BasicStructuredDisplayStorage,                                      "1.2.840.10008.5.1.4.1.1.131" },
			{ SOP::IntravascularOpticalCoherenceTomographyImageStorageForPresentation, "1.2.840.10008.5.1.4.1.1.14.1" },
			{ SOP::IntravascularOpticalCoherenceTomographyImageStorageForProcessing,   "1.2.840.10008.5.1.4.1.1.14.2" },
			{ SOP::CTImageStorage,                                                     "1.2.840.10008.5.1.4.1.1.2" },
			{ SOP::EnhancedCTImageStorage,                                             "1.2.840.10008.5.1.4.1.1.2.1" },
			{ SOP::LegacyConvertedEnhancedCTImageStorage,                              "1.2.840.10008.5.1.4.1.1.2.2" },
			{ SOP::NuclearMedicineImageStorage,                                        "1.2.840.10008.5.1.4.1.1.20" },
			{ SOP::CTDefinedProcedureProtocolStorage,                                  "1.2.840.10008.5.1.4.1.1.200.1" },
			{ SOP::CTPerformedProcedureProtocolStorage,                                "1.2.840.10008.5.1.4.1.1.200.2" },
			{ SOP::UltrasoundMultiframeImageStorage,                                   "1.2.840.10008.5.1.4.1.1.3" },
			{ SOP::UltrasoundMultiframeImageStorage,                                   "1.2.840.10008.5.1.4.1.1.3.1" },
			{ SOP::ParametricMapStorage,                                               "1.2.840.10008.5.1.4.1.1.30" },
			{ SOP::MRImageStorage,                                                     "1.2.840.10008.5.1.4.1.1.4" },
			{ SOP::EnhancedMRImageStorage,                                             "1.2.840.10008.5.1.4.1.1.4.1" },
			{ SOP::MRSpectroscopyStorage,                                              "1.2.840.10008.5.1.4.1.1.4.2" },
			{ SOP::EnhancedMRColorImageStorage,                                        "1.2.840.10008.5.1.4.1.1.4.3" },
			{ SOP::LegacyConvertedEnhancedMRImageStorage,                              "1.2.840.10008.5.1.4.1.1.4.4" },
			{ SOP::RTImageStorage,                                                     "1.2.840.10008.5.1.4.1.1.481.1" },
			{ SOP::RTDoseStorage,                                                      "1.2.840.10008.5.1.4.1.1.481.2" },
			{ SOP::RTStructureSetStorage,                                              "1.2.840.10008.5.1.4.1.1.481.3" },
			{ SOP::RTBeamsTreatmentRecordStorage,                                      "1.2.840.10008.5.1.4.1.1.481.4" },
			{ SOP::RTPlanStorage,                                                      "1.2.840.10008.5.1.4.1.1.481.5" },
			{ SOP::RTBrachyTreatmentRecordStorage,                                     "1.2.840.10008.5.1.4.1.1.481.6" },
			{ SOP::RTTreatmentSummaryRecordStorage,                                    "1.2.840.10008.5.1.4.1.1.481.7" },
			{ SOP::RTIonPlanStorage,                                                   "1.2.840.10008.5.1.4.1.1.481.8" },
			{ SOP::RTIonBeamsTreatmentRecordStorage,                                   "1.2.840.10008.5.1.4.1.1.481.9" },
			{ SOP::NuclearMedicineImageStorage,                                        "1.2.840.10008.5.1.4.1.1.5" },
			{ SOP::DICOSCTImageStorage,                                                "1.2.840.10008.5.1.4.1.1.501.1" },
			{ SOP::DICOSDigitalXRayImageStorageForPresentation,                        "1.2.840.10008.5.1.4.1.1.501.2.1" },
			{ SOP::DICOSDigitalXRayImageStorageForProcessing,                          "1.2.840.10008.5.1.4.1.1.501.2.2" },
			{ SOP::DICOSThreatDetectionReportStorage,                                  "1.2.840.10008.5.1.4.1.1.501.3" },
			{ SOP::DICOS2DAITStorage,                                                  "1.2.840.10008.5.1.4.1.1.501.4" },
			{ SOP::DICOS3DAITStorage,                                                  "1.2.840.10008.5.1.4.1.1.501.5" },
			{ SOP::DICOSQuadrupoleResonanceQRStorage,                                  "1.2.840.10008.5.1.4.1.1.501.6" },
			{ SOP::UltrasoundImageStorage,                                             "1.2.840.10008.5.1.4.1.1.6" },
			{ SOP::UltrasoundImageStorage,                                             "1.2.840.10008.5.1.4.1.1.6.1" },
			{ SOP::EnhancedUSVolumeStorage,                                            "1.2.840.10008.5.1.4.1.1.6.2" },
			{ SOP::EddyCurrentImageStorage,                                            "1.2.840.10008.5.1.4.1.1.601.1" },
			{ SOP::EddyCurrentMultiframeImageStorage,                                  "1.2.840.10008.5.1.4.1.1.601.2" },
			{ SOP::RawDataStorage,                                                     "1.2.840.10008.5.1.4.1.1.66" },
			{ SOP::SpatialRegistrationStorage,                                         "1.2.840.10008.5.1.4.1.1.66.1" },
			{ SOP::SpatialFiducialsStorage,                                            "1.2.840.10008.5.1.4.1.1.66.2" },
			{ SOP::DeformableSpatialRegistrationStorage,                               "1.2.840.10008.5.1.4.1.1.66.3" },
			{ SOP::SegmentationStorage,                                                "1.2.840.10008.5.1.4.1.1.66.4" },
			{ SOP::SurfaceSegmentationStorage,                                         "1.2.840.10008.5.1.4.1.1.66.5" },
			{ SOP::TractographyResultsStorage,                                         "1.2.840.10008.5.1.4.1.1.66.6" },
			{ SOP::RealWorldValueMappingStorage,                                       "1.2.840.10008.5.1.4.1.1.67" },
			{ SOP::SurfaceScanMeshStorage,                                             "1.2.840.10008.5.1.4.1.1.68.1" },
			{ SOP::SurfaceScanPointCloudStorage,                                       "1.2.840.10008.5.1.4.1.1.68.2" },
			{ SOP::SecondaryCaptureImageStorage,                                       "1.2.840.10008.5.1.4.1.1.7" },
			{ SOP::MultiframeSingleBitSecondaryCaptureImageStorage,                    "1.2.840.10008.5.1.4.1.1.7.1" },
			{ SOP::MultiframeGrayscaleByteSecondaryCaptureImageStorage,                "1.2.840.10008.5.1.4.1.1.7.2" },
			{ SOP::MultiframeGrayscaleWordSecondaryCaptureImageStorage,                "1.2.840.10008.5.1.4.1.1.7.3" },
			{ SOP::MultiframeTrueColorSecondaryCaptureImageStorage,                    "1.2.840.10008.5.1.4.1.1.7.4" },
			{ SOP::VLImageStorageTrial,                                                "1.2.840.10008.5.1.4.1.1.77.1" },
			{ SOP::VLEndoscopicImageStorage,                                           "1.2.840.10008.5.1.4.1.1.77.1.1" },
			{ SOP::VideoEndoscopicImageStorage,                                        "1.2.840.10008.5.1.4.1.1.77.1.1.1" },
			{ SOP::VLMicroscopicImageStorage,                                          "1.2.840.10008.5.1.4.1.1.77.1.2" },
			{ SOP::VideoMicroscopicImageStorage,                                       "1.2.840.10008.5.1.4.1.1.77.1.2.1" },
			{ SOP::VLSlideCoordinatesMicroscopicImageStorage,                          "1.2.840.10008.5.1.4.1.1.77.1.3" },
			{ SOP::VLPhotographicImageStorage,                                         "1.2.840.10008.5.1.4.1.1.77.1.4" },
			{ SOP::VideoPhotographicImageStorage,                                      "1.2.840.10008.5.1.4.1.1.77.1.4.1" },
			{ SOP::OphthalmicPhotography8BitImageStorage,                              "1.2.840.10008.5.1.4.1.1.77.1.5.1" },
			{ SOP::OphthalmicPhotography16BitImageStorage,                             "1.2.840.10008.5.1.4.1.1.77.1.5.2" },
			{ SOP::StereometricRelationshipStorage,                                    "1.2.840.10008.5.1.4.1.1.77.1.5.3" },
			{ SOP::OphthalmicTomographyImageStorage,                                   "1.2.840.10008.5.1.4.1.1.77.1.5.4" },
			{ SOP::WideFieldOphthalmicPhotographyStereographicProjectionImageStorage,  "1.2.840.10008.5.1.4.1.1.77.1.5.5" },
			{ SOP::WideFieldOphthalmicPhotography3DCoordinatesImageStorage,            "1.2.840.10008.5.1.4.1.1.77.1.5.6" },
			{ SOP::VLWholeSlideMicroscopyImageStorage,                                 "1.2.840.10008.5.1.4.1.1.77.1.6" },
			{ SOP::VLMultiframeImageStorageTrial,                                      "1.2.840.10008.5.1.4.1.1.77.2" },
			{ SOP::LensometryMeasurementsStorage,                                      "1.2.840.10008.5.1.4.1.1.78.1" },
			{ SOP::AutorefractionMeasurementsStorage,                                  "1.2.840.10008.5.1.4.1.1.78.2" },
			{ SOP::KeratometryMeasurementsStorage,                                     "1.2.840.10008.5.1.4.1.1.78.3" },
			{ SOP::SubjectiveRefractionMeasurementsStorage,                            "1.2.840.10008.5.1.4.1.1.78.4" },
			{ SOP::VisualAcuityMeasurementsStorage,                                    "1.2.840.10008.5.1.4.1.1.78.5" },
			{ SOP::SpectaclePrescriptionReportStorage,                                 "1.2.840.10008.5.1.4.1.1.78.6" },
			{ SOP::OphthalmicAxialMeasurementsStorage,                                 "1.2.840.10008.5.1.4.1.1.78.7" },
			{ SOP::IntraocularLensCalculationsStorage,                                 "1.2.840.10008.5.1.4.1.1.78.8" },
			{ SOP::MacularGridThicknessandVolumeReportStorage,                         "1.2.840.10008.5.1.4.1.1.79.1" },
			{ SOP::StandaloneOverlayStorage,                                           "1.2.840.10008.5.1.4.1.1.8" },
			{ SOP::OphthalmicVisualFieldStaticPerimetryMeasurementsStorage,            "1.2.840.10008.5.1.4.1.1.80.1" },
			{ SOP::OphthalmicThicknessMapStorage,                                      "1.2.840.10008.5.1.4.1.1.81.1" },
			{ SOP::CornealTopographyMapStorage,                                        "1.2.840.10008.5.1.4.1.1.82.1" },
			{ SOP::TextSRStorageTrial,                                                 "1.2.840.10008.5.1.4.1.1.88.1" },
			{ SOP::BasicTextSRStorage,                                                 "1.2.840.10008.5.1.4.1.1.88.11" },
			{ SOP::AudioSRStorageTrial,                                                "1.2.840.10008.5.1.4.1.1.88.2" },
			{ SOP::EnhancedSRStorage,                                                  "1.2.840.10008.5.1.4.1.1.88.22" },
			{ SOP::DetailSRStorageTrial,                                               "1.2.840.10008.5.1.4.1.1.88.3" },
			{ SOP::ComprehensiveSRStorage,                                             "1.2.840.10008.5.1.4.1.1.88.33" },
			{ SOP::Comprehensive3DSRStorage,                                           "1.2.840.10008.5.1.4.1.1.88.34" },
			{ SOP::ExtensibleSRStorage,                                                "1.2.840.10008.5.1.4.1.1.88.35" },
			{ SOP::ComprehensiveSRStorageTrial,                                        "1.2.840.10008.5.1.4.1.1.88.4" },
			{ SOP::ProcedureLogStorage,                                                "1.2.840.10008.5.1.4.1.1.88.40" },
			{ SOP::MammographyCADSRStorage,                                            "1.2.840.10008.5.1.4.1.1.88.50" },
			{ SOP::KeyObjectSelectionDocumentStorage,                                  "1.2.840.10008.5.1.4.1.1.88.59" },
			{ SOP::ChestCADSRStorage,                                                  "1.2.840.10008.5.1.4.1.1.88.65" },
			{ SOP::XRayRadiationDoseSRStorage,                                         "1.2.840.10008.5.1.4.1.1.88.67" },
			{ SOP::RadiopharmaceuticalRadiationDoseSRStorage,                          "1.2.840.10008.5.1.4.1.1.88.68" },
			{ SOP::ColonCADSRStorage,                                                  "1.2.840.10008.5.1.4.1.1.88.69" },
			{ SOP::ImplantationPlanSRStorage,                                          "1.2.840.10008.5.1.4.1.1.88.70" },
			{ SOP::AcquisitionContextSRStorage,                                        "1.2.840.10008.5.1.4.1.1.88.71" },
			{ SOP::SimplifiedAdultEchoSRStorage,                                       "1.2.840.10008.5.1.4.1.1.88.72" },
			{ SOP::StandaloneCurveStorage,                                             "1.2.840.10008.5.1.4.1.1.9" },
			{ SOP::WaveformStorageTrial,                                               "1.2.840.10008.5.1.4.1.1.9.1" },
			{ SOP::TwelveleadECGWaveformStorage,                                       "1.2.840.10008.5.1.4.1.1.9.1.1" },
			{ SOP::GeneralECGWaveformStorage,                                          "1.2.840.10008.5.1.4.1.1.9.1.2" },
			{ SOP::AmbulatoryECGWaveformStorage,                                       "1.2.840.10008.5.1.4.1.1.9.1.3" },
			{ SOP::HemodynamicWaveformStorage,                                         "1.2.840.10008.5.1.4.1.1.9.2.1" },
			{ SOP::CardiacElectrophysiologyWaveformStorage,                            "1.2.840.10008.5.1.4.1.1.9.3.1" },
			{ SOP::BasicVoiceAudioWaveformStorage,                                     "1.2.840.10008.5.1.4.1.1.9.4.1" },
			{ SOP::GeneralAudioWaveformStorage,                                        "1.2.840.10008.5.1.4.1.1.9.4.2" },
			{ SOP::ArterialPulseWaveformStorage,                                       "1.2.840.10008.5.1.4.1.1.9.5.1" },
			{ SOP::RespiratoryWaveformStorage,                                         "1.2.840.10008.5.1.4.1.1.9.6.1" },
			{ SOP::ContentAssessmentResultsStorage,                                    "1.2.840.10008.5.1.4.1.1.90.1" },
			{ SOP::RTBeamsDeliveryInstructionStorageTrial,                             "1.2.840.10008.5.1.4.34.1" },
			{ SOP::RTBrachyApplicationSetupDeliveryInstructionStorage,                 "1.2.840.10008.5.1.4.34.10" },
			{ SOP::RTBeamsDeliveryInstructionStorage,                                  "1.2.840.10008.5.1.4.34.7" },
			{ SOP::HangingProtocolStorage,                                             "1.2.840.10008.5.1.4.38.1" },
			{ SOP::GenericImplantTemplateStorage,                                      "1.2.840.10008.5.1.4.43.1" },
			{ SOP::ImplantAssemblyTemplateStorage,                                     "1.2.840.10008.5.1.4.44.1" },
			{ SOP::ImplantTemplateGroupStorage,                                        "1.2.840.10008.5.1.4.45.1" }
		};

		return sops;
	}

	using SOPClassMap = boost::container::flat_map<std::string, emdl::SOP_Class>;
	const SOPClassMap& getSOPClassMap()
	{
		static SOPClassMap sopClassMap = [] {
			SOPClassMap sopcm;
			const auto& sopcp = getSOPClassPairs();
			sopcm.reserve(sopcp.size());
			for (const auto& sop : sopcp)
				sopcm[sop.second] = sop.first;
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
			SOP::UltrasoundMultiframeImageStorage,
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
			SOP::NuclearMedicineImageStorage,
			SOP::DICOSCTImageStorage,
			SOP::DICOSDigitalXRayImageStorageForPresentation,
			SOP::DICOSDigitalXRayImageStorageForProcessing,
			SOP::DICOSThreatDetectionReportStorage,
			SOP::DICOS2DAITStorage,
			SOP::DICOS3DAITStorage,
			SOP::DICOSQuadrupoleResonanceQRStorage,
			SOP::UltrasoundImageStorage,
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
			SOP::TwelveleadECGWaveformStorage,
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
			SOP::ImplantTemplateGroupStorage,
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
			SOP::SimplifiedAdultEchoSRStorage,
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
			SOP::CompositingPlanarMPRVolumetricPresentationStateStorage,
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
			SOP::UltrasoundMultiframeImageStorage,
			SOP::NuclearMedicineImageStorage,
			SOP::UltrasoundImageStorage,
			SOP::VLImageStorageTrial,
			SOP::VLMultiframeImageStorageTrial,
			SOP::StandaloneOverlayStorage,
			SOP::TextSRStorageTrial,
			SOP::AudioSRStorageTrial,
			SOP::DetailSRStorageTrial,
			SOP::ComprehensiveSRStorageTrial,
			SOP::StandaloneCurveStorage,
			SOP::WaveformStorageTrial,
			SOP::RTBeamsDeliveryInstructionStorageTrial,
		};

		return retired;
	}

	using BitSet = std::vector<bool>;
	const BitSet& getStorageBitSet()
	{
		static BitSet storageBitSet = [] {
			BitSet bitSet;
			bitSet.assign(getSOPClassPairs().size(), false);
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
			bitSet.assign(getSOPClassPairs().size(), false);
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
			bitSet.assign(getSOPClassPairs().size(), false);
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
			bitSet.assign(getSOPClassPairs().size(), false);
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
			for (auto sop : getStructuredReportsBitSet())
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
		const auto& sops = getSOPClassPairs();
		int index = static_cast<int>(sop_class);
		return sops[index].second;
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
