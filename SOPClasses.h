#pragma once

#include <string>

namespace emdl
{

	enum class SOP_Class
	{
		Unknown, /// Not supported, or not recognized
		VerificationSOPClass,
		PatientRootQR_Find,
		PatientRootQR_Move,
		PatientRootQR_Get,
		StudyRootQR_Find,
		StudyRootQR_Move,
		StudyRootQR_Get,
		PatientStudyOnlyQR_Find,                 // Retired
		PatientStudyOnlyQR_Move,                 // Retired
		PatientStudyOnlyQR_Get,                  // Retired
		CompositeInstanceRootRetrieve_Move,
		CompositeInstanceRootRetrieve_Get,
		CompositeInstanceRootRetrieveWithoutBulkData_Get,
		ModalityWorklist_Find,
		GeneralPurposeWorklist_Find,
		StorageCommitmentPushModel,

		StoredPrintStorage,                      // Retired
		HardcopyGrayscaleImageStorage,           // Retired
		HardcopyColorImageStorage,               // Retired
		ComputedRadiographyImageStorage,
		DigitalXRayImageStorageForPresentation,
		DigitalXRayImageStorageForProcessing,
		DigitalMammographyXRayImageStorageForPresentation,
		DigitalMammographyXRayImageStorageForProcessing,
		DigitalIntraOralXRayImageStorageForPresentation,
		DigitalIntraOralXRayImageStorageForProcessing,
		CTImageStorage,
		EnhancedCTImageStorage,
		LegacyConvertedEnhancedCTImageStorage,
		RetiredUltrasoundMultiframeImageStorage, // Retired
		UltrasoundMultiframeImageStorage,
		MRImageStorage,
		EnhancedMRImageStorage,
		MRSpectroscopyStorage,
		EnhancedMRColorImageStorage,
		LegacyConvertedEnhancedMRImageStorage,
		RetiredNuclearMedicineImageStorage,      // Retired
		RetiredUltrasoundImageStorage,           // Retired
		UltrasoundImageStorage,
		EnhancedUSVolumeStorage,
		SecondaryCaptureImageStorage,
		MultiframeSingleBitSecondaryCaptureImageStorage,
		MultiframeGrayscaleByteSecondaryCaptureImageStorage,
		MultiframeGrayscaleWordSecondaryCaptureImageStorage,
		MultiframeTrueColorSecondaryCaptureImageStorage,
		StandaloneOverlayStorage,                // Retired
		StandaloneCurveStorage,                  // Retired
		TwelveLeadECGWaveformStorage,
		GeneralECGWaveformStorage,
		AmbulatoryECGWaveformStorage,
		HemodynamicWaveformStorage,
		CardiacElectrophysiologyWaveformStorage,
		BasicVoiceAudioWaveformStorage,
		GeneralAudioWaveformStorage,
		ArterialPulseWaveformStorage,
		RespiratoryWaveformStorage,
		StandaloneModalityLUTStorage,            // Retired
		StandaloneVOILUTStorage,                 // Retired
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
		XRayAngiographicBiPlaneImageStorage,     // Retired
		XRay3DAngiographicImageStorage,
		XRay3DCraniofacialImageStorage,
		BreastTomosynthesisImageStorage,
		BreastProjectionXRayImageStorageForPresentation,
		BreastProjectionXRayImageStorageForProcessing,
		IntravascularOpticalCoherenceTomographyImageStorageForPresentation,
		IntravascularOpticalCoherenceTomographyImageStorageForProcessing,
		NuclearMedicineImageStorage,
		ParametricMapStorage,
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
		VLImageStorage,                          // Retired
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
		VLMultiFrameImageStorage,                // Retired
		LensometryMeasurementsStorage,
		AutorefractionMeasurementsStorage,
		KeratometryMeasurementsStorage,
		SubjectiveRefractionMeasurementsStorage,
		VisualAcuityMeasurementsStorage,
		SpectaclePrescriptionReportStorage,
		OphthalmicAxialMeasurementsStorage,
		IntraocularLensCalculationsStorage,
		MacularGridThicknessAndVolumeReportStorage,
		OphthalmicVisualFieldStaticPerimetryMeasurementsStorage,
		OphthalmicThicknessMapStorage,
		CornealTopographyMapStorage,
		BasicTextSRStorage,
		EnhancedSRStorage,
		ComprehensiveSRStorage,
		Comprehensive3DSRStorage,
		ExtensibleSRStorage,
		ProcedureLogStorage,
		MammographyCADSRStorage,
		KeyObjectSelectionDocumentStorage,
		ChestCADSRStorage,
		XRayRadiationDoseSRStorage,
		RadiopharmaceuticalRadiationDoseSRStorage,
		ColonCADSRStorage,
		ImplantationPlanSRDocumentStorage,
		AcquisitionContextSRStorage,
		ContentAssessmentResultsStorage,
		EncapsulatedPDFStorage,
		EncapsulatedCDAStorage,
		PositronEmissionTomographyImageStorage,
		LegacyConvertedEnhancedPETImageStorage,
		StandalonePETCurveStorage,               // Retired
		EnhancedPETImageStorage,
		BasicStructuredDisplayStorage,
		RTImageStorage,
		RTDoseStorage,
		RTStructureSetStorage,
		RTBeamsTreatmentRecordStorage,
		RTPlanStorage,
		RTBrachyTreatmentRecordStorage,
		RTTreatmentSummaryRecordStorage,
		RTIonPlanStorage,
		RTIonBeamsTreatmentRecordStorage,
		RTBeamsDeliveryInstructionStorage,
		RTBrachyApplicationSetupDeliveryInstructionStorage,
		HangingProtocolStorage,
		GenericImplantTemplateStorage,
		ImplantAssemblyTemplateStorage,
		ImplantTemplateGroupStorage
	};

	SOP_Class getSOPClass(const std::string& uid);
	const std::string& getSOPClassUID(SOP_Class sop_class);

	bool isStorage(SOP_Class sop);
	bool isImage(SOP_Class sop);
	bool isStructuredReport(SOP_Class sop);
	bool isPresentationState(SOP_Class sop);

}
