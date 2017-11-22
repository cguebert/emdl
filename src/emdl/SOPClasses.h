#pragma once

#include <emdl/emdl_api.h>

#include <string>
#include <vector>

namespace emdl
{
	// clang-format off
	enum class SOP_Class
	{
		Unknown, /// Not supported, or not recognized
		Verification,
		StorageCommitmentPushModel,
		StorageCommitmentPullModel,               // Retired
		ProceduralEventLogging,
		SubstanceAdministrationLogging,
		BasicStudyContentNotification,            // Retired
		DetachedPatientManagement,                // Retired
		DetachedPatientManagementMeta,            // Retired
		DetachedVisitManagement,                  // Retired
		DetachedStudyManagement,                  // Retired
		StudyComponentManagement,                 // Retired
		ModalityPerformedProcedureStep,
		ModalityPerformedProcedureStepRetrieve,
		ModalityPerformedProcedureStepNotification,
		DetachedResultsManagement,                // Retired
		DetachedResultsManagementMeta,            // Retired
		DetachedStudyManagementMeta,              // Retired
		DetachedInterpretationManagement,         // Retired
		BasicFilmSession,
		PrintJob,
		BasicAnnotationBox,
		Printer,
		PrinterConfigurationRetrieval,
		BasicColorPrintManagementMeta,
		ReferencedColorPrintManagementMeta,       // Retired
		BasicFilmBox,
		VOILUTBox,
		PresentationLUT,
		ImageOverlayBox,                          // Retired
		BasicPrintImageOverlayBox,                // Retired
		PrintQueueManagement,                     // Retired
		PullPrintRequest,                         // Retired
		PullStoredPrintManagementMeta,            // Retired
		MediaCreationManagementUID,
		BasicGrayscaleImageBox,
		BasicColorImageBox,
		ReferencedImageBox,                       // Retired
		DisplaySystem,
		BasicGrayscalePrintManagementMeta,
		ReferencedGrayscalePrintManagementMeta,   // Retired
		PatientRootQR_Find,
		PatientRootQR_Move,
		PatientRootQR_Get,
		StudyRootQR_Find,
		StudyRootQR_Move,
		StudyRootQR_Get,
		PatientStudyOnlyQR_Find,                  // Retired
		PatientStudyOnlyQR_Move,                  // Retired
		PatientStudyOnlyQR_Get,                   // Retired
		CompositeInstanceRootRetrieve_Move,
		CompositeInstanceRootRetrieve_Get,
		CompositeInstanceRetrieveWithoutBulkData_Get,
		DefinedProcedureProtocol_Find,
		DefinedProcedureProtocol_Move,
		DefinedProcedureProtocol_Get,
		ModalityWorklist_Find,
		GeneralPurposeWorklistManagementMeta,     // Retired
		GeneralPurposeWorklist_Find,              // Retired
		GeneralPurposeScheduledProcedureStep,     // Retired
		GeneralPurposePerformedProcedureStep,     // Retired
		InstanceAvailabilityNotification,
		RTConventionalMachineVerificationTrial,   // Retired
		RTIonMachineVerificationTrial,            // Retired
		UnifiedProcedureStepPushTrial,            // Retired
		UnifiedProcedureStepWatchTrial,           // Retired
		UnifiedProcedureStepPullTrial,            // Retired
		UnifiedProcedureStepEventTrial,           // Retired
		UnifiedProcedureStepPush,
		UnifiedProcedureStepWatch,
		UnifiedProcedureStepPull,
		UnifiedProcedureStepEvent,
		RTConventionalMachineVerification,
		RTIonMachineVerification,
		GeneralRelevantPatientInformationQuery,
		BreastImagingRelevantPatientInformationQuery,
		CardiacRelevantPatientInformationQuery,
		HangingProtocol_Find,
		HangingProtocol_Move,
		HangingProtocol_Get,
		ColorPaletteQR_Find,
		ColorPaletteQR_Move,
		ColorPaletteQR_Get,
		ProductCharacteristicsQuery,
		SubstanceApprovalQuery,
		GenericImplantTemplate_Find,
		GenericImplantTemplate_Move,
		GenericImplantTemplate_Get,
		ImplantAssemblyTemplate_Find,
		ImplantAssemblyTemplate_Move,
		ImplantAssemblyTemplate_Get,
		ImplantTemplateGroup_Find,
		ImplantTemplateGroup_Move,
		ImplantTemplateGroup_Get,

		MediaStorageDirectoryStorage,
		StoredPrintStorage,                       // Retired
		HardcopyGrayscaleImageStorage,            // Retired
		HardcopyColorImageStorage,                // Retired
		ComputedRadiographyImageStorage,
		DigitalXRayImageStorageForPresentation,
		DigitalXRayImageStorageForProcessing,
		DigitalMammographyXRayImageStorageForPresentation,
		DigitalMammographyXRayImageStorageForProcessing,
		DigitalIntraOralXRayImageStorageForPresentation,
		DigitalIntraOralXRayImageStorageForProcessing,
		StandaloneModalityLUTStorage,             // Retired
		EncapsulatedPDFStorage,
		EncapsulatedCDAStorage,
		StandaloneVOILUTStorage,                  // Retired
		GrayscaleSoftcopyPresentationStateStorage,
		ColorSoftcopyPresentationStateStorage,
		PseudoColorSoftcopyPresentationStateStorage,
		BlendingSoftcopyPresentationStateStorage,
		XAXRFGrayscaleSoftcopyPresentationStateStorage,
		GrayscalePlanarMPRVolumetricPresentationStateStorage,
		CompositingPlanarMPRVolumetricPresentationStateStorage,
		XRayAngiographicImageStorage,
		EnhancedXAImageStorage,
		XRayRadiofluoroscopicImageStorage,
		EnhancedXRFImageStorage,
		XRayAngiographicBiPlaneImageStorage,      // Retired
		PositronEmissionTomographyImageStorage,
		LegacyConvertedEnhancedPETImageStorage,
		StandalonePETCurveStorage,                // Retired
		XRay3DAngiographicImageStorage,
		XRay3DCraniofacialImageStorage,
		BreastTomosynthesisImageStorage,
		BreastProjectionXRayImageStorageForPresentation,
		BreastProjectionXRayImageStorageForProcessing,
		EnhancedPETImageStorage,
		BasicStructuredDisplayStorage,
		IntravascularOpticalCoherenceTomographyImageStorageForPresentation,
		IntravascularOpticalCoherenceTomographyImageStorageForProcessing,
		CTImageStorage,
		EnhancedCTImageStorage,
		LegacyConvertedEnhancedCTImageStorage,
		NuclearMedicineImageStorage,
		CTDefinedProcedureProtocolStorage,
		CTPerformedProcedureProtocolStorage,
		UltrasoundMultiframeImageStorage_Retired, // Retired
		UltrasoundMultiframeImageStorage,
		ParametricMapStorage,
		MRImageStorage,
		EnhancedMRImageStorage,
		MRSpectroscopyStorage,
		EnhancedMRColorImageStorage,
		LegacyConvertedEnhancedMRImageStorage,
		RTImageStorage,
		RTDoseStorage,
		RTStructureSetStorage,
		RTBeamsTreatmentRecordStorage,
		RTPlanStorage,
		RTBrachyTreatmentRecordStorage,
		RTTreatmentSummaryRecordStorage,
		RTIonPlanStorage,
		RTIonBeamsTreatmentRecordStorage,
		NuclearMedicineImageStorage_Retired,      // Retired
		DICOSCTImageStorage,
		DICOSDigitalXRayImageStorageForPresentation,
		DICOSDigitalXRayImageStorageForProcessing,
		DICOSThreatDetectionReportStorage,
		DICOS2DAITStorage,
		DICOS3DAITStorage,
		DICOSQuadrupoleResonanceQRStorage,
		UltrasoundImageStorage_Retired,           // Retired
		UltrasoundImageStorage,
		EnhancedUSVolumeStorage,
		EddyCurrentImageStorage,
		EddyCurrentMultiframeImageStorage,
		RawDataStorage,
		SpatialRegistrationStorage,
		SpatialFiducialsStorage,
		DeformableSpatialRegistrationStorage,
		SegmentationStorage,
		SurfaceSegmentationStorage,
		TractographyResultsStorage,
		RealWorldValueMappingStorage,
		SurfaceScanMeshStorage,
		SurfaceScanPointCloudStorage,
		SecondaryCaptureImageStorage,
		MultiframeSingleBitSecondaryCaptureImageStorage,
		MultiframeGrayscaleByteSecondaryCaptureImageStorage,
		MultiframeGrayscaleWordSecondaryCaptureImageStorage,
		MultiframeTrueColorSecondaryCaptureImageStorage,
		VLImageStorageTrial,                      // Retired
		VLEndoscopicImageStorage,
		VideoEndoscopicImageStorage,
		VLMicroscopicImageStorage,
		VideoMicroscopicImageStorage,
		VLSlideCoordinatesMicroscopicImageStorage,
		VLPhotographicImageStorage,
		VideoPhotographicImageStorage,
		OphthalmicPhotography8BitImageStorage,
		OphthalmicPhotography16BitImageStorage,
		StereometricRelationshipStorage,
		OphthalmicTomographyImageStorage,
		WideFieldOphthalmicPhotographyStereographicProjectionImageStorage,
		WideFieldOphthalmicPhotography3DCoordinatesImageStorage,
		VLWholeSlideMicroscopyImageStorage,
		VLMultiframeImageStorageTrial,            // Retired
		LensometryMeasurementsStorage,
		AutorefractionMeasurementsStorage,
		KeratometryMeasurementsStorage,
		SubjectiveRefractionMeasurementsStorage,
		VisualAcuityMeasurementsStorage,
		SpectaclePrescriptionReportStorage,
		OphthalmicAxialMeasurementsStorage,
		IntraocularLensCalculationsStorage,
		MacularGridThicknessandVolumeReportStorage,
		StandaloneOverlayStorage,                 // Retired
		OphthalmicVisualFieldStaticPerimetryMeasurementsStorage,
		OphthalmicThicknessMapStorage,
		CornealTopographyMapStorage,
		TextSRStorageTrial,                       // Retired
		BasicTextSRStorage,
		AudioSRStorageTrial,                      // Retired
		EnhancedSRStorage,
		DetailSRStorageTrial,                     // Retired
		ComprehensiveSRStorage,
		Comprehensive3DSRStorage,
		ExtensibleSRStorage,
		ComprehensiveSRStorageTrial,              // Retired
		ProcedureLogStorage,
		MammographyCADSRStorage,
		KeyObjectSelectionDocumentStorage,
		ChestCADSRStorage,
		XRayRadiationDoseSRStorage,
		RadiopharmaceuticalRadiationDoseSRStorage,
		ColonCADSRStorage,
		ImplantationPlanSRStorage,
		AcquisitionContextSRStorage,
		SimplifiedAdultEchoSRStorage,
		StandaloneCurveStorage,                   // Retired
		WaveformStorageTrial,                     // Retired
		TwelveLeadECGWaveformStorage,
		GeneralECGWaveformStorage,
		AmbulatoryECGWaveformStorage,
		HemodynamicWaveformStorage,
		CardiacElectrophysiologyWaveformStorage,
		BasicVoiceAudioWaveformStorage,
		GeneralAudioWaveformStorage,
		ArterialPulseWaveformStorage,
		RespiratoryWaveformStorage,
		ContentAssessmentResultsStorage,
		RTBeamsDeliveryInstructionStorageTrial,   // Retired
		RTBrachyApplicationSetupDeliveryInstructionStorage,
		RTBeamsDeliveryInstructionStorage,
		HangingProtocolStorage,
		GenericImplantTemplateStorage,
		ImplantAssemblyTemplateStorage,
		ImplantTemplateGroupStorage
	};
	// clang-format on

	EMDL_API SOP_Class getSOPClass(const std::string& uid);
	EMDL_API const std::string& getSOPClassUID(SOP_Class sop_class);
	EMDL_API const std::string& getSOPClassDescription(SOP_Class sop_class);
	EMDL_API const std::vector<SOP_Class>& getSOPClassList();

	EMDL_API bool isStorage(SOP_Class sop);
	EMDL_API bool isImage(SOP_Class sop);
	EMDL_API bool isStructuredReport(SOP_Class sop);
	EMDL_API bool isPresentationState(SOP_Class sop);
	EMDL_API bool isRetired(SOP_Class sop);
}
