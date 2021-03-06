﻿using Dynamo.Controls;
using Dynamo.Graph.Nodes;
using Dynamo.Interfaces;
using Dynamo.Models;
using Dynamo.Scheduler;
using Dynamo.ViewModels;
using Dynamo.Wpf.ViewModels.Watch3D;
using DynamoShapeManager;
using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;

namespace DynamoSandboxWrapper
{
    public class DynamoWrapper
    {
        private DynamoViewModel dynamoViewModel;

        public void LoadDynamo(string asmLocation)
        {
            AppDomain.CurrentDomain.AssemblyResolve += ResolveAssembly;

            //Include Dynamo Core path in System Path variable for helix to load properly.
            UpdateSystemPathForProcess();

            StartUI();
        }
        
        public void CreateSelectionNode()
        {
            if (dynamoViewModel == null)
                return;

            var command = new DynamoModel.CreateNodeCommand(new[] { Guid.NewGuid() }, "Fusion.SelectEntity", 0, 0, true, false);
            dynamoViewModel.ExecuteCommand(command);
        }

        public void CreateCustomSelectionNode(NodeModel node)
        {
            if (dynamoViewModel == null)
                return;

            var command = new DynamoModel.CreateNodeCommand(node, 0, 0, true, false);
            dynamoViewModel.ExecuteCommand(command);
        }

        private static string DynamoCorePath
        {
            get
            {
                // TODO: Use DynamoInstallDetective to find core path from Dynamo install folder
                // OR use config file with the core path listed 
                return @"C:\Users\pratapa.ADS\Documents\GitHub\Dynamo\bin\AnyCPU\Debug";
            }
        }

        private static Assembly ResolveAssembly(object sender, ResolveEventArgs args)
        {
            var assemblyPath = string.Empty;
            var assemblyName = new AssemblyName(args.Name).Name + ".dll";

            try
            {
                assemblyPath = Path.Combine(DynamoCorePath, assemblyName);
                if (File.Exists(assemblyPath))
                    return Assembly.LoadFrom(assemblyPath);

                var assemblyLocation = Assembly.GetExecutingAssembly().Location;
                var assemblyDirectory = Path.GetDirectoryName(assemblyLocation);

                assemblyPath = Path.Combine(assemblyDirectory, assemblyName);
                return (File.Exists(assemblyPath) ? Assembly.LoadFrom(assemblyPath) : null);
            }
            catch (Exception ex)
            {
                throw new Exception(string.Format("The location of the assembly, {0} could not be resolved for loading.", assemblyPath), ex);
            }
        }
        private void StartUI()
        {
            try
            {
                var nodeLibraryPath = @"C:\Users\pratapa.ADS\AppData\Roaming\Autodesk\Autodesk Fusion 360\API\AddIns\DynamoFusion\Debug\";

                //var customNodeLibraryPath = @"C:\Users\pratapa.ADS\AppData\Roaming\Autodesk\Autodesk Fusion 360\API\AddIns\DynamoFusion\Debug\";
                var customNodeLibraryPath = string.Empty;

                var model = MakeModel(nodeLibraryPath, customNodeLibraryPath);

                dynamoViewModel = DynamoViewModel.Start(
                    new DynamoViewModel.StartConfiguration()
                    {
                        CommandFilePath = string.Empty,
                        DynamoModel = model,
                        Watch3DViewModel = HelixWatch3DViewModel.TryCreateHelixWatch3DViewModel(new Watch3DViewModelStartupParams(model), model.Logger),
                        ShowLogin = true
                    });

                var view = new DynamoView(dynamoViewModel);
                view.Show();
            }
            catch (Exception e)
            {
                try
                {
#if DEBUG
                    // Display the recorded command XML when the crash happens, 
                    // so that it maybe saved and re-run later
                    if (dynamoViewModel != null)
                        dynamoViewModel.SaveRecordedCommand.Execute(null);
#endif

                    //DynamoModel.IsCrashing = true;
                    //InstrumentationLogger.LogException(e);
                    //StabilityTracking.GetInstance().NotifyCrash();

                    //if (viewModel != null)
                    //{
                    //    // Show the unhandled exception dialog so user can copy the 
                    //    // crash details and report the crash if she chooses to.
                    //    viewModel.Model.OnRequestsCrashPrompt(null,
                    //        new CrashPromptArgs(e.Message + "\n\n" + e.StackTrace));

                    //    // Give user a chance to save (but does not allow cancellation)
                    //    viewModel.Exit(allowCancel: false);
                    //}
                }
                catch
                {
                }

                Debug.WriteLine(e.Message);
                Debug.WriteLine(e.StackTrace);
            }
        }

        private static DynamoModel MakeModel(string nodeLibraryPath, string customNodeLibraryPath)
        {
            var version = LibraryVersion.Version222;
            var libGFolderName = string.Format("libg_{0}", ((int)version));
            var preloaderLocation = Path.Combine(DynamoCorePath, libGFolderName);
            var geometryFactoryPath = Path.Combine(preloaderLocation, DynamoShapeManager.Utilities.GeometryFactoryAssembly);

            var config = new DynamoModel.DefaultStartConfiguration()
            {
                GeometryFactoryPath = geometryFactoryPath,
                ProcessMode = TaskProcessMode.Asynchronous
            };

            config.UpdateManager = null;
            config.StartInTestMode = false;
            
            config.PathResolver = new FusionPathResolver(nodeLibraryPath, customNodeLibraryPath) as IPathResolver;

            var model = DynamoModel.Start(config);
            return model;
        }

        private static void UpdateSystemPathForProcess()
        {
            var path =
                    Environment.GetEnvironmentVariable(
                        "Path",
                        EnvironmentVariableTarget.Process) + ";" + DynamoCorePath;
            Environment.SetEnvironmentVariable("Path", path, EnvironmentVariableTarget.Process);
        }
    }
}
