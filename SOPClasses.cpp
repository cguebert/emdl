#include "SOPClasses.h"

#include <boost/container/flat_map.hpp>

#include <algorithm>
#include <vector>

namespace
{

	using SOPClassPair = std::pair<emds::SOP_Class, std::string>;
	using SOPClassPairs = std::vector<SOPClassPair>;
	const SOPClassPairs& getSOPClassPairs()
	{
		using SOP = emds::SOP_Class;
		static SOPClassPairs sops = {
			{ SOP::Unknown,                                                            "Not supported" },
			{ SOP::VerificationSOPClass,                                               "1.2.840.10008.1.1" },
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
			{ SOP::CompositeInstanceRootRetrieveWithoutBulkData_Get,                   "1.2.840.10008.5.1.4.1.2.5.3" },
			{ SOP::ModalityWorklist_Find,                                              "1.2.840.10008.5.1.4.31" },
			{ SOP::GeneralPurposeWorklist_Find,                                        "1.2.840.10008.5.1.4.32.1" },
			{ SOP::StorageCommitmentPushModel,                                         "1.2.840.10008.1.20.1" },

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
			{ SOP::CTImageStorage,                                                     "1.2.840.10008.5.1.4.1.1.2" },
			{ SOP::EnhancedCTImageStorage,                                             "1.2.840.10008.5.1.4.1.1.2.1" },
			{ SOP::LegacyConvertedEnhancedCTImageStorage,                              "1.2.840.10008.5.1.4.1.1.2.2" },
			{ SOP::UltrasoundMultiframeImageStorage,                                   "1.2.840.10008.5.1.4.1.1.3" },
			{ SOP::UltrasoundMultiframeImageStorage,                                   "1.2.840.10008.5.1.4.1.1.3.1" },
			{ SOP::MRImageStorage,                                                     "1.2.840.10008.5.1.4.1.1.4" },
			{ SOP::EnhancedMRImageStorage,                                             "1.2.840.10008.5.1.4.1.1.4.1" },
			{ SOP::MRSpectroscopyStorage,                                              "1.2.840.10008.5.1.4.1.1.4.2" },
			{ SOP::EnhancedMRColorImageStorage,                                        "1.2.840.10008.5.1.4.1.1.4.3" },
			{ SOP::LegacyConvertedEnhancedMRImageStorage,                              "1.2.840.10008.5.1.4.1.1.4.4" },
			{ SOP::NuclearMedicineImageStorage,                                        "1.2.840.10008.5.1.4.1.1.5" },
			{ SOP::UltrasoundImageStorage,                                             "1.2.840.10008.5.1.4.1.1.6" },
			{ SOP::UltrasoundImageStorage,                                             "1.2.840.10008.5.1.4.1.1.6.1" },
			{ SOP::EnhancedUSVolumeStorage,                                            "1.2.840.10008.5.1.4.1.1.6.2" },
			{ SOP::SecondaryCaptureImageStorage,                                       "1.2.840.10008.5.1.4.1.1.7" },
			{ SOP::MultiframeSingleBitSecondaryCaptureImageStorage,                    "1.2.840.10008.5.1.4.1.1.7.1" },
			{ SOP::MultiframeGrayscaleByteSecondaryCaptureImageStorage,                "1.2.840.10008.5.1.4.1.1.7.2" },
			{ SOP::MultiframeGrayscaleWordSecondaryCaptureImageStorage,                "1.2.840.10008.5.1.4.1.1.7.3" },
			{ SOP::MultiframeTrueColorSecondaryCaptureImageStorage,                    "1.2.840.10008.5.1.4.1.1.7.4" },
			{ SOP::StandaloneOverlayStorage,                                           "1.2.840.10008.5.1.4.1.1.8" },
			{ SOP::StandaloneCurveStorage,                                             "1.2.840.10008.5.1.4.1.1.9" },
			{ SOP::TwelveLeadECGWaveformStorage,                                       "1.2.840.10008.5.1.4.1.1.9.1.1" },
			{ SOP::GeneralECGWaveformStorage,                                          "1.2.840.10008.5.1.4.1.1.9.1.2" },
			{ SOP::AmbulatoryECGWaveformStorage,                                       "1.2.840.10008.5.1.4.1.1.9.1.3" },
			{ SOP::HemodynamicWaveformStorage,                                         "1.2.840.10008.5.1.4.1.1.9.2.1" },
			{ SOP::CardiacElectrophysiologyWaveformStorage,                            "1.2.840.10008.5.1.4.1.1.9.3.1" },
			{ SOP::BasicVoiceAudioWaveformStorage,                                     "1.2.840.10008.5.1.4.1.1.9.4.1" },
			{ SOP::GeneralAudioWaveformStorage,                                        "1.2.840.10008.5.1.4.1.1.9.4.2" },
			{ SOP::ArterialPulseWaveformStorage,                                       "1.2.840.10008.5.1.4.1.1.9.5.1" },
			{ SOP::RespiratoryWaveformStorage,                                         "1.2.840.10008.5.1.4.1.1.9.6.1" },
			{ SOP::StandaloneModalityLUTStorage,                                       "1.2.840.10008.5.1.4.1.1.10" },
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
			{ SOP::XRay3DAngiographicImageStorage,                                     "1.2.840.10008.5.1.4.1.1.13.1.1" },
			{ SOP::XRay3DCraniofacialImageStorage,                                     "1.2.840.10008.5.1.4.1.1.13.1.2" },
			{ SOP::BreastTomosynthesisImageStorage,                                    "1.2.840.10008.5.1.4.1.1.13.1.3" },
			{ SOP::BreastProjectionXRayImageStorageForPresentation,                    "1.2.840.10008.5.1.4.1.1.13.1.4" },
			{ SOP::BreastProjectionXRayImageStorageForProcessing,                      "1.2.840.10008.5.1.4.1.1.13.1.5" },
			{ SOP::IntravascularOpticalCoherenceTomographyImageStorageForPresentation, "1.2.840.10008.5.1.4.1.1.14.1" },
			{ SOP::IntravascularOpticalCoherenceTomographyImageStorageForProcessing,   "1.2.840.10008.5.1.4.1.1.14.2" },
			{ SOP::NuclearMedicineImageStorage,                                        "1.2.840.10008.5.1.4.1.1.20" },
			{ SOP::ParametricMapStorage,                                               "1.2.840.10008.5.1.4.1.1.30" },
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
			{ SOP::VLImageStorage,                                                     "1.2.840.10008.5.1.4.1.1.77.1" },
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
			{ SOP::VLMultiFrameImageStorage,                                           "1.2.840.10008.5.1.4.1.1.77.2" },
			{ SOP::LensometryMeasurementsStorage,                                      "1.2.840.10008.5.1.4.1.1.78.1" },
			{ SOP::AutorefractionMeasurementsStorage,                                  "1.2.840.10008.5.1.4.1.1.78.2" },
			{ SOP::KeratometryMeasurementsStorage,                                     "1.2.840.10008.5.1.4.1.1.78.3" },
			{ SOP::SubjectiveRefractionMeasurementsStorage,                            "1.2.840.10008.5.1.4.1.1.78.4" },
			{ SOP::VisualAcuityMeasurementsStorage,                                    "1.2.840.10008.5.1.4.1.1.78.5" },
			{ SOP::SpectaclePrescriptionReportStorage,                                 "1.2.840.10008.5.1.4.1.1.78.6" },
			{ SOP::OphthalmicAxialMeasurementsStorage,                                 "1.2.840.10008.5.1.4.1.1.78.7" },
			{ SOP::IntraocularLensCalculationsStorage,                                 "1.2.840.10008.5.1.4.1.1.78.8" },
			{ SOP::MacularGridThicknessAndVolumeReportStorage,                         "1.2.840.10008.5.1.4.1.1.79.1" },
			{ SOP::OphthalmicVisualFieldStaticPerimetryMeasurementsStorage,            "1.2.840.10008.5.1.4.1.1.80.1" },
			{ SOP::OphthalmicThicknessMapStorage,                                      "1.2.840.10008.5.1.4.1.1.81.1" },
			{ SOP::CornealTopographyMapStorage,                                        "1.2.840.10008.5.1.4.1.1.82.1" },
			{ SOP::BasicTextSRStorage,                                                 "1.2.840.10008.5.1.4.1.1.88.11" },
			{ SOP::EnhancedSRStorage,                                                  "1.2.840.10008.5.1.4.1.1.88.22" },
			{ SOP::ComprehensiveSRStorage,                                             "1.2.840.10008.5.1.4.1.1.88.33" },
			{ SOP::Comprehensive3DSRStorage,                                           "1.2.840.10008.5.1.4.1.1.88.34" },
			{ SOP::ExtensibleSRStorage,                                                "1.2.840.10008.5.1.4.1.1.88.35" },
			{ SOP::ProcedureLogStorage,                                                "1.2.840.10008.5.1.4.1.1.88.40" },
			{ SOP::MammographyCADSRStorage,                                            "1.2.840.10008.5.1.4.1.1.88.50" },
			{ SOP::KeyObjectSelectionDocumentStorage,                                  "1.2.840.10008.5.1.4.1.1.88.59" },
			{ SOP::ChestCADSRStorage,                                                  "1.2.840.10008.5.1.4.1.1.88.65" },
			{ SOP::XRayRadiationDoseSRStorage,                                         "1.2.840.10008.5.1.4.1.1.88.67" },
			{ SOP::RadiopharmaceuticalRadiationDoseSRStorage,                          "1.2.840.10008.5.1.4.1.1.88.68" },
			{ SOP::ColonCADSRStorage,                                                  "1.2.840.10008.5.1.4.1.1.88.69" },
			{ SOP::ImplantationPlanSRDocumentStorage,                                  "1.2.840.10008.5.1.4.1.1.88.70" },
			{ SOP::AcquisitionContextSRStorage,                                        "1.2.840.10008.5.1.4.1.1.88.71" },
			{ SOP::ContentAssessmentResultsStorage,                                    "1.2.840.10008.5.1.4.1.1.90.1" },
			{ SOP::EncapsulatedPDFStorage,                                             "1.2.840.10008.5.1.4.1.1.104.1" },
			{ SOP::EncapsulatedCDAStorage,                                             "1.2.840.10008.5.1.4.1.1.104.2" },
			{ SOP::PositronEmissionTomographyImageStorage,                             "1.2.840.10008.5.1.4.1.1.128" },
			{ SOP::LegacyConvertedEnhancedPETImageStorage,                             "1.2.840.10008.5.1.4.1.1.128.1" },
			{ SOP::StandalonePETCurveStorage,                                          "1.2.840.10008.5.1.4.1.1.129" },
			{ SOP::EnhancedPETImageStorage,                                            "1.2.840.10008.5.1.4.1.1.130" },
			{ SOP::BasicStructuredDisplayStorage,                                      "1.2.840.10008.5.1.4.1.1.131" },
			{ SOP::RTImageStorage,                                                     "1.2.840.10008.5.1.4.1.1.481.1" },
			{ SOP::RTDoseStorage,                                                      "1.2.840.10008.5.1.4.1.1.481.2" },
			{ SOP::RTStructureSetStorage,                                              "1.2.840.10008.5.1.4.1.1.481.3" },
			{ SOP::RTBeamsTreatmentRecordStorage,                                      "1.2.840.10008.5.1.4.1.1.481.4" },
			{ SOP::RTPlanStorage,                                                      "1.2.840.10008.5.1.4.1.1.481.5" },
			{ SOP::RTBrachyTreatmentRecordStorage,                                     "1.2.840.10008.5.1.4.1.1.481.6" },
			{ SOP::RTTreatmentSummaryRecordStorage,                                    "1.2.840.10008.5.1.4.1.1.481.7" },
			{ SOP::RTIonPlanStorage,                                                   "1.2.840.10008.5.1.4.1.1.481.8" },
			{ SOP::RTIonBeamsTreatmentRecordStorage,                                   "1.2.840.10008.5.1.4.1.1.481.9" },
			{ SOP::RTBeamsDeliveryInstructionStorage,                                  "1.2.840.10008.5.1.4.34.7" },
			{ SOP::RTBrachyApplicationSetupDeliveryInstructionStorage,                 "1.2.840.10008.5.1.4.34.10" },
			{ SOP::HangingProtocolStorage,                                             "1.2.840.10008.5.1.4.38.1" },
			{ SOP::GenericImplantTemplateStorage,                                      "1.2.840.10008.5.1.4.43.1" },
			{ SOP::ImplantAssemblyTemplateStorage,                                     "1.2.840.10008.5.1.4.44.1" },
			{ SOP::ImplantTemplateGroupStorage,                                        "1.2.840.10008.5.1.4.45.1" }
		};

		return sops;
	}

	using SOPClassMap = boost::container::flat_map<std::string, emds::SOP_Class>;
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

	using SOPClasses = std::vector<emds::SOP_Class>;
	const SOPClasses& getStorageClasses()
	{
		using SOP = emds::SOP_Class;
		static SOPClasses storageClasses = {
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
			SOP::CTImageStorage,
			SOP::EnhancedCTImageStorage,
			SOP::LegacyConvertedEnhancedCTImageStorage,
			SOP::UltrasoundMultiframeImageStorage,
			SOP::UltrasoundMultiframeImageStorage,
			SOP::MRImageStorage,
			SOP::EnhancedMRImageStorage,
			SOP::MRSpectroscopyStorage,
			SOP::EnhancedMRColorImageStorage,
			SOP::LegacyConvertedEnhancedMRImageStorage,
			SOP::NuclearMedicineImageStorage,
			SOP::UltrasoundImageStorage,
			SOP::UltrasoundImageStorage,
			SOP::EnhancedUSVolumeStorage,
			SOP::SecondaryCaptureImageStorage,
			SOP::MultiframeSingleBitSecondaryCaptureImageStorage,
			SOP::MultiframeGrayscaleByteSecondaryCaptureImageStorage,
			SOP::MultiframeGrayscaleWordSecondaryCaptureImageStorage,
			SOP::MultiframeTrueColorSecondaryCaptureImageStorage,
			SOP::StandaloneOverlayStorage,
			SOP::StandaloneCurveStorage,
			SOP::TwelveLeadECGWaveformStorage,
			SOP::GeneralECGWaveformStorage,
			SOP::AmbulatoryECGWaveformStorage,
			SOP::HemodynamicWaveformStorage,
			SOP::CardiacElectrophysiologyWaveformStorage,
			SOP::BasicVoiceAudioWaveformStorage,
			SOP::GeneralAudioWaveformStorage,
			SOP::ArterialPulseWaveformStorage,
			SOP::RespiratoryWaveformStorage,
			SOP::StandaloneModalityLUTStorage,
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
			SOP::XRay3DAngiographicImageStorage,
			SOP::XRay3DCraniofacialImageStorage,
			SOP::BreastTomosynthesisImageStorage,
			SOP::BreastProjectionXRayImageStorageForPresentation,
			SOP::BreastProjectionXRayImageStorageForProcessing,
			SOP::IntravascularOpticalCoherenceTomographyImageStorageForPresentation,
			SOP::IntravascularOpticalCoherenceTomographyImageStorageForProcessing,
			SOP::NuclearMedicineImageStorage,
			SOP::ParametricMapStorage,
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
			SOP::VLImageStorage,
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
			SOP::VLMultiFrameImageStorage,
			SOP::LensometryMeasurementsStorage,
			SOP::AutorefractionMeasurementsStorage,
			SOP::KeratometryMeasurementsStorage,
			SOP::SubjectiveRefractionMeasurementsStorage,
			SOP::VisualAcuityMeasurementsStorage,
			SOP::SpectaclePrescriptionReportStorage,
			SOP::OphthalmicAxialMeasurementsStorage,
			SOP::IntraocularLensCalculationsStorage,
			SOP::MacularGridThicknessAndVolumeReportStorage,
			SOP::OphthalmicVisualFieldStaticPerimetryMeasurementsStorage,
			SOP::OphthalmicThicknessMapStorage,
			SOP::CornealTopographyMapStorage,
			SOP::BasicTextSRStorage,
			SOP::EnhancedSRStorage,
			SOP::ComprehensiveSRStorage,
			SOP::Comprehensive3DSRStorage,
			SOP::ExtensibleSRStorage,
			SOP::ProcedureLogStorage,
			SOP::MammographyCADSRStorage,
			SOP::KeyObjectSelectionDocumentStorage,
			SOP::ChestCADSRStorage,
			SOP::XRayRadiationDoseSRStorage,
			SOP::RadiopharmaceuticalRadiationDoseSRStorage,
			SOP::ColonCADSRStorage,
			SOP::ImplantationPlanSRDocumentStorage,
			SOP::AcquisitionContextSRStorage,
			SOP::ContentAssessmentResultsStorage,
			SOP::EncapsulatedPDFStorage,
			SOP::EncapsulatedCDAStorage,
			SOP::PositronEmissionTomographyImageStorage,
			SOP::LegacyConvertedEnhancedPETImageStorage,
			SOP::StandalonePETCurveStorage,
			SOP::EnhancedPETImageStorage,
			SOP::BasicStructuredDisplayStorage,
			SOP::RTImageStorage,
			SOP::RTDoseStorage,
			SOP::RTStructureSetStorage,
			SOP::RTBeamsTreatmentRecordStorage,
			SOP::RTPlanStorage,
			SOP::RTBrachyTreatmentRecordStorage,
			SOP::RTTreatmentSummaryRecordStorage,
			SOP::RTIonPlanStorage,
			SOP::RTIonBeamsTreatmentRecordStorage,
			SOP::RTBeamsDeliveryInstructionStorage,
			SOP::RTBrachyApplicationSetupDeliveryInstructionStorage,
			SOP::HangingProtocolStorage,
			SOP::GenericImplantTemplateStorage,
			SOP::ImplantAssemblyTemplateStorage,
			SOP::ImplantTemplateGroupStorage
		};

		return storageClasses;
	}

	const SOPClasses& getStructuredReportsClasses()
	{
		using SOP = emds::SOP_Class;
		static SOPClasses structuredReports = {
			SOP::BasicTextSRStorage,
			SOP::EnhancedSRStorage,
			SOP::ComprehensiveSRStorage,
			SOP::Comprehensive3DSRStorage,
			SOP::ExtensibleSRStorage,
			SOP::ProcedureLogStorage,
			SOP::MammographyCADSRStorage,
			SOP::KeyObjectSelectionDocumentStorage,
			SOP::ChestCADSRStorage,
			SOP::XRayRadiationDoseSRStorage,
			SOP::RadiopharmaceuticalRadiationDoseSRStorage,
			SOP::ColonCADSRStorage,
			SOP::ImplantationPlanSRDocumentStorage,
			SOP::AcquisitionContextSRStorage
		};

		return structuredReports;
	}

	const SOPClasses& getPresentationStatesClasses()
	{
		using SOP = emds::SOP_Class;
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

namespace emds
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

}
